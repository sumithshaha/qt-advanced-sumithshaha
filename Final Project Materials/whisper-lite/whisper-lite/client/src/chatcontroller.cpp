#include "chatcontroller.h"

#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QJsonArray>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcCtrl, "whisper.ctrl")

ChatController::ChatController(QObject *parent)
    : QObject(parent)
{
    connect(&m_ws, &WsClient::messageReceived,
            this, &ChatController::onServerMessage);
    connect(&m_ws, &WsClient::connected,
            this, &ChatController::onWsConnected);
    connect(&m_ws, &WsClient::stateChanged,
            this, &ChatController::onWsStateChanged);
    connect(&m_ws, &WsClient::lastErrorChanged, this, [this] {
        setLastError(m_ws.lastError());
    });
}

ChatController::~ChatController() = default;

void ChatController::start(const QString &nickname, const QUrl &serverUrl)
{
    m_myNick = nickname.trimmed();
    if (m_myNick.isEmpty()) {
        setLastError(QStringLiteral("Nickname cannot be empty"));
        return;
    }
    emit myNicknameChanged();

    // Open per-user SQLite database
    const QString dataDir =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QString dbPath = QDir(dataDir).filePath(
        QStringLiteral("whisper-%1.db").arg(m_myNick));
    if (!m_store.open(dbPath)) {
        setLastError(QStringLiteral("Failed to open message database"));
        return;
    }
    qCInfo(lcCtrl) << "Opened store at" << dbPath;

    // Connect WebSocket
    m_ws.connectTo(serverUrl, m_myNick);
}

void ChatController::logout()
{
    m_ws.close();
    m_users.clear();
    m_messages.clear();
    m_store.close();
    m_myNick.clear();
    m_activePeer.clear();
    emit myNicknameChanged();
    emit activePeerChanged();
}

void ChatController::sendText(const QString &peer, const QString &text)
{
    if (peer.isEmpty() || text.isEmpty()) return;
    if (peer == m_myNick) {
        setLastError(QStringLiteral("Cannot send messages to yourself"));
        return;
    }

    const qint64 ts = QDateTime::currentMSecsSinceEpoch();

    // Build envelope. Wire field is "text" (plaintext) — the architectural
    // seam where E2EE would re-enter is right here: encrypt(text) into a
    // "blob" field instead.
    QJsonObject env{
        { "type", "message" },
        { "to",   peer },
        { "text", text },
        { "seq",  ++m_seq },
        { "ts",   ts },
    };
    m_ws.sendJson(env);

    // Persist locally
    ChatMessage msg;
    msg.id        = m_store.insertMessage(peer, /*incoming=*/false, text, ts);
    msg.peer      = peer;
    msg.text      = text;
    msg.timestamp = ts;
    msg.incoming  = false;
    m_messages.appendMessage(msg);
    emit messageDelivered(peer);
}

void ChatController::selectPeer(const QString &peer)
{
    setActivePeer(peer);
}

void ChatController::setActivePeer(const QString &peer)
{
    if (m_activePeer == peer) return;
    m_activePeer = peer;
    m_messages.setActivePeer(peer);
    m_messages.setMessages(m_store.loadConversation(peer));
    emit activePeerChanged();
}

void ChatController::onServerMessage(const QJsonObject &obj)
{
    const QString type = obj.value(QStringLiteral("type")).toString();
    if      (type == QLatin1String("roster"))   handleRoster(obj);
    else if (type == QLatin1String("presence")) handlePresence(obj);
    else if (type == QLatin1String("message"))  handleMessage(obj);
    else if (type == QLatin1String("error"))    handleError(obj);
    else if (type == QLatin1String("pong"))     { /* ignore */ }
    else qCWarning(lcCtrl) << "Unknown server message type:" << type;
}

void ChatController::handleRoster(const QJsonObject &obj)
{
    const QJsonArray users = obj.value(QStringLiteral("users")).toArray();
    qCInfo(lcCtrl) << "Roster with" << users.size() << "users";
    for (const QJsonValue &v : users) {
        const QJsonObject u = v.toObject();
        UserInfo info;
        info.nick      = u.value(QStringLiteral("nick")).toString();
        info.online    = true;
        m_users.setUser(info);
    }
}

void ChatController::handlePresence(const QJsonObject &obj)
{
    UserInfo info;
    info.nick   = obj.value(QStringLiteral("nick")).toString();
    info.online = obj.value(QStringLiteral("online")).toBool();

    if (info.online) {
        m_users.setUser(info);
        qCInfo(lcCtrl) << "Online:" << info.nick;
    } else {
        m_users.removeUser(info.nick);  // marks offline
        qCInfo(lcCtrl) << "Offline:" << info.nick;
    }
}

void ChatController::handleMessage(const QJsonObject &obj)
{
    const QString from = obj.value(QStringLiteral("from")).toString();
    const QString text = obj.value(QStringLiteral("text")).toString();
    const qint64 ts = obj.value(QStringLiteral("ts")).toVariant().toLongLong();

    if (from.isEmpty() || text.isEmpty()) {
        qCWarning(lcCtrl) << "Malformed message envelope, dropping";
        return;
    }

    ChatMessage msg;
    msg.id        = m_store.insertMessage(from, /*incoming=*/true, text, ts);
    msg.peer      = from;
    msg.text      = text;
    msg.timestamp = ts;
    msg.incoming  = true;
    m_messages.appendMessage(msg);

    m_notif.showMessage(from, text);
}

void ChatController::handleError(const QJsonObject &obj)
{
    const QString reason = obj.value(QStringLiteral("reason")).toString();
    setLastError(QStringLiteral("Server error: %1").arg(reason));
}

void ChatController::onWsConnected()
{
    qCInfo(lcCtrl) << "WebSocket connected";
}

void ChatController::onWsStateChanged()
{
    emit statusChanged();
}

QString ChatController::status() const
{
    switch (m_ws.state()) {
    case WsClient::Disconnected: return QStringLiteral("Disconnected");
    case WsClient::Connecting:   return QStringLiteral("Connecting…");
    case WsClient::Connected:    return QStringLiteral("Connected");
    case WsClient::Reconnecting: return QStringLiteral("Reconnecting (attempt %1)…")
                                            .arg(m_ws.reconnectAttempt());
    case WsClient::Failed:       return QStringLiteral("Connection failed");
    }
    return {};
}

void ChatController::setLastError(const QString &err)
{
    if (m_lastError == err) return;
    m_lastError = err;
    qCWarning(lcCtrl) << "lastError:" << err;
    emit lastErrorChanged();
}
