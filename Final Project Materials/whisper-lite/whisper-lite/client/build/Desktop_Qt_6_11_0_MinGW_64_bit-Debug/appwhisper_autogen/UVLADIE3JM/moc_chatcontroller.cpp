/****************************************************************************
** Meta object code from reading C++ file 'chatcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/chatcontroller.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN14ChatControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto ChatController::qt_create_metaobjectdata<qt_meta_tag_ZN14ChatControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ChatController",
        "QML.Element",
        "auto",
        "myNicknameChanged",
        "",
        "statusChanged",
        "activePeerChanged",
        "lastErrorChanged",
        "messageDelivered",
        "peer",
        "onServerMessage",
        "QJsonObject",
        "obj",
        "onWsConnected",
        "onWsStateChanged",
        "start",
        "nickname",
        "QUrl",
        "serverUrl",
        "logout",
        "sendText",
        "text",
        "selectPeer",
        "myNickname",
        "status",
        "messages",
        "MessageModel*",
        "users",
        "UserModel*",
        "activePeer",
        "wsState",
        "lastError"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'myNicknameChanged'
        QtMocHelpers::SignalData<void()>(3, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'statusChanged'
        QtMocHelpers::SignalData<void()>(5, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'activePeerChanged'
        QtMocHelpers::SignalData<void()>(6, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'lastErrorChanged'
        QtMocHelpers::SignalData<void()>(7, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'messageDelivered'
        QtMocHelpers::SignalData<void(const QString &)>(8, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Slot 'onServerMessage'
        QtMocHelpers::SlotData<void(const QJsonObject &)>(10, 4, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Slot 'onWsConnected'
        QtMocHelpers::SlotData<void()>(13, 4, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onWsStateChanged'
        QtMocHelpers::SlotData<void()>(14, 4, QMC::AccessPrivate, QMetaType::Void),
        // Method 'start'
        QtMocHelpers::MethodData<void(const QString &, const QUrl &)>(15, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 16 }, { 0x80000000 | 17, 18 },
        }}),
        // Method 'logout'
        QtMocHelpers::MethodData<void()>(19, 4, QMC::AccessPublic, QMetaType::Void),
        // Method 'sendText'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(20, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::QString, 21 },
        }}),
        // Method 'selectPeer'
        QtMocHelpers::MethodData<void(const QString &)>(22, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'myNickname'
        QtMocHelpers::PropertyData<QString>(23, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'status'
        QtMocHelpers::PropertyData<QString>(24, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
        // property 'messages'
        QtMocHelpers::PropertyData<MessageModel*>(25, 0x80000000 | 26, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'users'
        QtMocHelpers::PropertyData<UserModel*>(27, 0x80000000 | 28, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'activePeer'
        QtMocHelpers::PropertyData<QString>(29, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'wsState'
        QtMocHelpers::PropertyData<int>(30, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
        // property 'lastError'
        QtMocHelpers::PropertyData<QString>(31, QMetaType::QString, QMC::DefaultPropertyFlags, 3),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
    });
    return QtMocHelpers::metaObjectData<ChatController, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject ChatController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ChatControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ChatControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14ChatControllerE_t>.metaTypes,
    nullptr
} };

void ChatController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ChatController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->myNicknameChanged(); break;
        case 1: _t->statusChanged(); break;
        case 2: _t->activePeerChanged(); break;
        case 3: _t->lastErrorChanged(); break;
        case 4: _t->messageDelivered((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->onServerMessage((*reinterpret_cast<std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 6: _t->onWsConnected(); break;
        case 7: _t->onWsStateChanged(); break;
        case 8: _t->start((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[2]))); break;
        case 9: _t->logout(); break;
        case 10: _t->sendText((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->selectPeer((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ChatController::*)()>(_a, &ChatController::myNicknameChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ChatController::*)()>(_a, &ChatController::statusChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ChatController::*)()>(_a, &ChatController::activePeerChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ChatController::*)()>(_a, &ChatController::lastErrorChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ChatController::*)(const QString & )>(_a, &ChatController::messageDelivered, 4))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MessageModel* >(); break;
        case 3:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< UserModel* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->myNickname(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->status(); break;
        case 2: *reinterpret_cast<MessageModel**>(_v) = _t->messages(); break;
        case 3: *reinterpret_cast<UserModel**>(_v) = _t->users(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->activePeer(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->wsState(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->lastError(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 4: _t->setActivePeer(*reinterpret_cast<QString*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *ChatController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ChatControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ChatController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void ChatController::myNicknameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ChatController::statusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ChatController::activePeerChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ChatController::lastErrorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ChatController::messageDelivered(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}
QT_WARNING_POP
