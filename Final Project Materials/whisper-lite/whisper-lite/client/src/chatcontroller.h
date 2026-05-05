#pragma once

#include <QObject>
#include <QUrl>
#include <QJsonObject>
#include <QString>

#ifdef WHISPER_QML_BUILD
#include <QtQmlIntegration>
#endif

#include "wsclient.h"
#include "messagestore.h"
#include "messagemodel.h"
#include "usermodel.h"
#include "notificationhelper.h"

/**
 * ChatController — single QML-facing orchestrator.
 *
 * Scope note: this build is "Whisper-Lite". TLS protects the wire from
 * passive observers; the server itself sees plaintext (i.e. the relay is
 * trusted). End-to-end encryption is documented as deferred work in
 * docs/SELF_ASSESSMENT.md. The architectural seams are kept in place so
 * adding libsodium back is a localised change to this file plus a new
 * CryptoManager class.
 */
class ChatController : public QObject
{
    Q_OBJECT
#ifdef WHISPER_QML_BUILD
    QML_ELEMENT
#endif

    Q_PROPERTY(QString myNickname READ myNickname NOTIFY myNicknameChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(MessageModel* messages READ messages CONSTANT)
    Q_PROPERTY(UserModel* users READ users CONSTANT)
    Q_PROPERTY(QString activePeer READ activePeer WRITE setActivePeer NOTIFY activePeerChanged)
    Q_PROPERTY(int wsState READ wsState NOTIFY statusChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)

public:
    explicit ChatController(QObject *parent = nullptr);
    ~ChatController() override;

    Q_INVOKABLE void start(const QString &nickname, const QUrl &serverUrl);
    Q_INVOKABLE void logout();
    Q_INVOKABLE void sendText(const QString &peer, const QString &text);
    Q_INVOKABLE void selectPeer(const QString &peer);

    QString myNickname() const { return m_myNick; }
    QString status() const;
    int wsState() const { return m_ws.state(); }
    MessageModel *messages() { return &m_messages; }
    UserModel *users() { return &m_users; }
    QString activePeer() const { return m_activePeer; }
    void setActivePeer(const QString &peer);
    QString lastError() const { return m_lastError; }

signals:
    void myNicknameChanged();
    void statusChanged();
    void activePeerChanged();
    void lastErrorChanged();
    void messageDelivered(const QString &peer);

private slots:
    void onServerMessage(const QJsonObject &obj);
    void onWsConnected();
    void onWsStateChanged();

private:
    void handleRoster(const QJsonObject &obj);
    void handlePresence(const QJsonObject &obj);
    void handleMessage(const QJsonObject &obj);
    void handleError(const QJsonObject &obj);

    void setLastError(const QString &err);

    WsClient            m_ws;
    MessageStore        m_store;
    MessageModel        m_messages;
    UserModel           m_users;
    NotificationHelper  m_notif;

    QString m_myNick;
    QString m_activePeer;
    QString m_lastError;
    qint64  m_seq = 0;
};
