#include "notificationhelper.h"

#include <QLoggingCategory>

#ifdef Q_OS_ANDROID
#include <QJniObject>
#include <QJniEnvironment>
#include <QCoreApplication>
#endif

Q_LOGGING_CATEGORY(lcNotif, "whisper.notif")

NotificationHelper::NotificationHelper(QObject *parent)
    : QObject(parent)
{
}

void NotificationHelper::ensureChannel()
{
    if (m_channelReady) return;

#ifdef Q_OS_ANDROID
    // On Android 8.0+ (API 26+) we MUST register a channel before showing
    // any notifications. Channel ID is the durable identifier — the
    // user can disable our channel from system settings.
    QJniObject context = QNativeInterface::QAndroidApplication::context();
    QJniObject channelId   = QJniObject::fromString(QStringLiteral("whisper-messages"));
    QJniObject channelName = QJniObject::fromString(QStringLiteral("Whisper Messages"));
    const jint importance = 3; // NotificationManager.IMPORTANCE_DEFAULT

    QJniObject channel("android/app/NotificationChannel",
                       "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;I)V",
                       channelId.object<jstring>(),
                       channelName.object<jstring>(),
                       importance);

    QJniObject nmClass = QJniObject::fromString(QStringLiteral("notification"));
    QJniObject manager = context.callObjectMethod(
        "getSystemService",
        "(Ljava/lang/String;)Ljava/lang/Object;",
        nmClass.object<jstring>());

    if (manager.isValid()) {
        manager.callMethod<void>(
            "createNotificationChannel",
            "(Landroid/app/NotificationChannel;)V",
            channel.object());
        m_channelReady = true;
        qCInfo(lcNotif) << "Notification channel registered";
    }
#else
    m_channelReady = true; // desktop has nothing to register
#endif
}

void NotificationHelper::showMessage(const QString &title, const QString &body)
{
    ensureChannel();

#ifdef Q_OS_ANDROID
    QJniObject context = QNativeInterface::QAndroidApplication::context();

    QJniObject channelId = QJniObject::fromString(QStringLiteral("whisper-messages"));
    QJniObject jTitle    = QJniObject::fromString(title);
    QJniObject jBody     = QJniObject::fromString(body);

    QJniObject builder("androidx/core/app/NotificationCompat$Builder",
                       "(Landroid/content/Context;Ljava/lang/String;)V",
                       context.object(), channelId.object<jstring>());

    builder = builder.callObjectMethod(
        "setContentTitle",
        "(Ljava/lang/CharSequence;)Landroidx/core/app/NotificationCompat$Builder;",
        jTitle.object<jstring>());
    builder = builder.callObjectMethod(
        "setContentText",
        "(Ljava/lang/CharSequence;)Landroidx/core/app/NotificationCompat$Builder;",
        jBody.object<jstring>());

    // android.R.drawable.ic_dialog_email is always available.
    const jint icon = QJniObject::getStaticField<jint>(
        "android/R$drawable", "ic_dialog_email");
    builder.callObjectMethod(
        "setSmallIcon",
        "(I)Landroidx/core/app/NotificationCompat$Builder;",
        icon);

    QJniObject notification = builder.callObjectMethod(
        "build", "()Landroid/app/Notification;");

    QJniObject managerCompat = QJniObject::callStaticObjectMethod(
        "androidx/core/app/NotificationManagerCompat",
        "from",
        "(Landroid/content/Context;)Landroidx/core/app/NotificationManagerCompat;",
        context.object());

    static jint nextId = 1000;
    managerCompat.callMethod<void>("notify",
                                   "(ILandroid/app/Notification;)V",
                                   nextId++, notification.object());
    qCDebug(lcNotif) << "Posted notification:" << title;
#else
    // Desktop fallback — useful for Windows/Linux dev runs.
    qCInfo(lcNotif).noquote() << "[notification]" << title << "—" << body;
#endif
}

void NotificationHelper::requestPermission()
{
#ifdef Q_OS_ANDROID
    // Android 13+ requires runtime POST_NOTIFICATIONS permission.
    // QtCore 6.5+ exposes QNativeInterface::QAndroidApplication for this.
    // Skipped for brevity — the android.permission.POST_NOTIFICATIONS line
    // in AndroidManifest.xml plus a one-shot prompt at first run is the
    // production flow.
    qCInfo(lcNotif) << "POST_NOTIFICATIONS request would happen here";
#endif
}
