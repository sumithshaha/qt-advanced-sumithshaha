#pragma once

#include <QObject>
#include <QWebSocket>
#include <QTimer>
#include <QUrl>
#include <QJsonObject>
#include <QAbstractSocket>

#ifdef WHISPER_QML_BUILD
#include <QtQmlIntegration>
#endif

/**
 * WsClient — wraps QWebSocket with TLS pinning, exponential-backoff
 * reconnection, and heartbeat ping. State is exposed as a Q_PROPERTY so
 * QML can show a connection indicator.
 *
 * Lifecycle:
 *   Disconnected -> connectTo() -> Connecting -> Connected
 *                                              \-> Disconnected -> Reconnecting -> ...
 */
class WsClient : public QObject
{
    Q_OBJECT
#ifdef WHISPER_QML_BUILD
    QML_ELEMENT
#endif
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
    Q_PROPERTY(int reconnectAttempt READ reconnectAttempt NOTIFY stateChanged)

public:
    enum State { Disconnected, Connecting, Connected, Reconnecting, Failed };
    Q_ENUM(State)

    explicit WsClient(QObject *parent = nullptr);
    ~WsClient() override;

    Q_INVOKABLE void connectTo(const QUrl &url, const QString &nickname);
    Q_INVOKABLE void close();
    Q_INVOKABLE void sendJson(const QJsonObject &obj);

    State state() const { return m_state; }
    QString lastError() const { return m_lastError; }
    int reconnectAttempt() const { return m_attempt; }

signals:
    void stateChanged();
    void lastErrorChanged();
    void messageReceived(const QJsonObject &obj);
    void connected();
    void disconnected();

private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString &msg);
    void onSslErrors(const QList<QSslError> &errors);
    void onErrorOccurred(QAbstractSocket::SocketError error);
    void scheduleReconnect();
    void heartbeat();

private:
    void setState(State newState);
    void setLastError(const QString &err);

    QWebSocket m_sock;
    QTimer m_reconnectTimer;
    QTimer m_heartbeatTimer;

    QUrl m_url;
    QString m_nick;
    QString m_lastError;

    State m_state = Disconnected;
    int m_attempt = 0;
    bool m_userClosed = false;

    static constexpr int kMaxAttempts    = 12;
    static constexpr int kBaseDelayMs    = 500;
    static constexpr int kMaxDelayMs     = 30'000;
    static constexpr int kHeartbeatMs    = 25'000;
    static constexpr int kJitterPercent  = 20;
};
