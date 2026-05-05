#pragma once

#include <QObject>
#include <QString>

#ifdef WHISPER_QML_BUILD
#include <QtQmlIntegration>
#endif

/**
 * NotificationHelper — shows a system notification when a new message arrives
 * while the app isn't in the foreground.
 *
 * On Android we use Qt's JNI integration (QJniObject) to call
 * NotificationManager.notify() with a Notification.Builder. On desktop
 * (where we develop and test the C++ logic) we fall back to a stderr log
 * line so the rest of the code path stays uniform.
 *
 * Channel ID 'whisper-messages' is registered once at startup with
 * IMPORTANCE_DEFAULT so notifications make sound but don't pop heads-up.
 */
class NotificationHelper : public QObject
{
    Q_OBJECT
#ifdef WHISPER_QML_BUILD
    QML_ELEMENT
#endif

public:
    explicit NotificationHelper(QObject *parent = nullptr);

    Q_INVOKABLE void showMessage(const QString &title, const QString &body);
    Q_INVOKABLE void requestPermission();   // Android 13+ runtime POST_NOTIFICATIONS

private:
    void ensureChannel();
    bool m_channelReady = false;
};
