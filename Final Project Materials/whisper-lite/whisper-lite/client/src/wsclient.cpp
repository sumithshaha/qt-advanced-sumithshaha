#include "wsclient.h"
#include "tlsconfig.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QSslError>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcWs, "whisper.ws")

WsClient::WsClient(QObject *parent)
    : QObject(parent)
{
    m_sock.setSslConfiguration(TlsConfig::pinned());

    connect(&m_sock, &QWebSocket::connected,
            this, &WsClient::onConnected);
    connect(&m_sock, &QWebSocket::disconnected,
            this, &WsClient::onDisconnected);
    connect(&m_sock, &QWebSocket::textMessageReceived,
            this, &WsClient::onTextMessageReceived);
    connect(&m_sock, &QWebSocket::sslErrors,
            this, &WsClient::onSslErrors);
    connect(&m_sock, &QWebSocket::errorOccurred,
            this, &WsClient::onErrorOccurred);

    m_reconnectTimer.setSingleShot(true);
    connect(&m_reconnectTimer, &QTimer::timeout, this, [this] {
        if (m_userClosed) return;
        setState(Connecting);
        m_sock.open(m_url);
    });

    m_heartbeatTimer.setInterval(kHeartbeatMs);
    connect(&m_heartbeatTimer, &QTimer::timeout, this, &WsClient::heartbeat);
}

WsClient::~WsClient() = default;

void WsClient::connectTo(const QUrl &url, const QString &nickname)
{
    m_url = url;
    m_nick = nickname;
    m_userClosed = false;
    m_attempt = 0;
    setLastError({});
    setState(Connecting);
    qCInfo(lcWs) << "Connecting to" << url.toString() << "as" << nickname;
    m_sock.open(url);
}

void WsClient::close()
{
    m_userClosed = true;
    m_reconnectTimer.stop();
    m_heartbeatTimer.stop();
    m_sock.close();
    setState(Disconnected);
}

void WsClient::sendJson(const QJsonObject &obj)
{
    if (m_state != Connected) {
        qCWarning(lcWs) << "sendJson while not connected, dropping type="
                        << obj.value("type").toString();
        return;
    }
    const QByteArray payload =
        QJsonDocument(obj).toJson(QJsonDocument::Compact);
    m_sock.sendTextMessage(QString::fromUtf8(payload));
}

void WsClient::onConnected()
{
    m_attempt = 0;
    setState(Connected);
    m_heartbeatTimer.start();

    // Send the hello envelope: nickname only (lite build, no E2EE)
    QJsonObject hello{
        { "type", "hello" },
        { "nick", m_nick }
    };
    sendJson(hello);
    emit connected();
    qCInfo(lcWs) << "Connected and sent hello as" << m_nick;
}

void WsClient::onDisconnected()
{
    m_heartbeatTimer.stop();
    qCInfo(lcWs) << "Disconnected (close code:" << m_sock.closeCode()
                 << "reason:" << m_sock.closeReason() << ")";
    emit disconnected();

    if (m_userClosed) {
        setState(Disconnected);
    } else {
        scheduleReconnect();
    }
}

void WsClient::onTextMessageReceived(const QString &msg)
{
    QJsonParseError pe;
    const auto doc = QJsonDocument::fromJson(msg.toUtf8(), &pe);
    if (pe.error != QJsonParseError::NoError) {
        qCWarning(lcWs) << "Malformed JSON from server:" << pe.errorString();
        return;
    }
    if (!doc.isObject()) {
        qCWarning(lcWs) << "Top-level JSON is not an object";
        return;
    }
    emit messageReceived(doc.object());
}

void WsClient::onSslErrors(const QList<QSslError> &errors)
{
    qCInfo(lcWs) << "SSL errors:" << errors.size();
    // QWebSocket exposes the peer certificate via its SSL configuration,
    // not as a top-level method (unlike QSslSocket).
    if (TlsConfig::acceptablePinned(m_sock.sslConfiguration().peerCertificate(), errors)) {
        m_sock.ignoreSslErrors();
    } else {
        setLastError(QStringLiteral("TLS pin/error rejected — refusing to connect"));
        m_userClosed = true;   // Don't reconnect on a pin failure
        m_sock.abort();
        setState(Failed);
    }
}

void WsClient::onErrorOccurred(QAbstractSocket::SocketError error)
{
    setLastError(m_sock.errorString());
    qCWarning(lcWs) << "Socket error" << error << ":" << m_sock.errorString();
    // Disconnected slot will fire shortly and trigger reconnect.
}

void WsClient::scheduleReconnect()
{
    if (m_userClosed) return;

    if (++m_attempt > kMaxAttempts) {
        setLastError(QStringLiteral("Giving up after %1 reconnect attempts")
                     .arg(kMaxAttempts));
        setState(Failed);
        return;
    }

    // Exponential backoff: 500ms, 1s, 2s, 4s, 8s, 16s, 30s (capped) ...
    const int exp   = qMin(m_attempt, 6);
    const int base  = qMin(kMaxDelayMs, kBaseDelayMs * (1 << exp));
    const int jitter = QRandomGenerator::global()->bounded(
        (base * kJitterPercent) / 100);
    const int delay = base + jitter;

    qCInfo(lcWs) << "Reconnect attempt" << m_attempt << "of" << kMaxAttempts
                 << "in" << delay << "ms";
    setState(Reconnecting);
    m_reconnectTimer.start(delay);
}

void WsClient::heartbeat()
{
    m_sock.ping();  // QWebSocket sends a control frame; server replies with pong
}

void WsClient::setState(State newState)
{
    if (m_state == newState) return;
    m_state = newState;
    emit stateChanged();
}

void WsClient::setLastError(const QString &err)
{
    if (m_lastError == err) return;
    m_lastError = err;
    emit lastErrorChanged();
}