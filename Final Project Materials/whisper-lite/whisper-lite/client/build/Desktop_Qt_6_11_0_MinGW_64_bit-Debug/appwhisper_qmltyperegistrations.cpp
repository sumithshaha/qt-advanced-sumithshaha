/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#if __has_include(<chatcontroller.h>)
#  include <chatcontroller.h>
#endif
#if __has_include(<messagemodel.h>)
#  include <messagemodel.h>
#endif
#if __has_include(<notificationhelper.h>)
#  include <notificationhelper.h>
#endif
#if __has_include(<settings.h>)
#  include <settings.h>
#endif
#if __has_include(<usermodel.h>)
#  include <usermodel.h>
#endif
#if __has_include(<wsclient.h>)
#  include <wsclient.h>
#endif


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_Whisper()
{
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    qmlRegisterTypesAndRevisions<ChatController>("Whisper", 1);
    qmlRegisterTypesAndRevisions<MessageModel>("Whisper", 1);
    qmlRegisterTypesAndRevisions<NotificationHelper>("Whisper", 1);
    QMetaType::fromType<QAbstractItemModel *>().id();
    qmlRegisterEnum<QAbstractItemModel::LayoutChangeHint>("QAbstractItemModel::LayoutChangeHint");
    qmlRegisterEnum<QAbstractItemModel::CheckIndexOption>("QAbstractItemModel::CheckIndexOption");
    QMetaType::fromType<QAbstractListModel *>().id();
    qmlRegisterTypesAndRevisions<Settings>("Whisper", 1);
    qmlRegisterTypesAndRevisions<UserModel>("Whisper", 1);
    qmlRegisterTypesAndRevisions<WsClient>("Whisper", 1);
    qmlRegisterEnum<WsClient::State>("WsClient::State");
    QT_WARNING_POP
    qmlRegisterModule("Whisper", 1, 0);
}

static const QQmlModuleRegistration whisperRegistration("Whisper", qml_register_types_Whisper);
