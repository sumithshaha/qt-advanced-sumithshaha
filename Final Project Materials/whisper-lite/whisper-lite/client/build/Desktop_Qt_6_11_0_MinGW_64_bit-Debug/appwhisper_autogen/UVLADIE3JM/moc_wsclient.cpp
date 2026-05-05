/****************************************************************************
** Meta object code from reading C++ file 'wsclient.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/wsclient.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wsclient.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN8WsClientE_t {};
} // unnamed namespace

template <> constexpr inline auto WsClient::qt_create_metaobjectdata<qt_meta_tag_ZN8WsClientE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "WsClient",
        "QML.Element",
        "auto",
        "stateChanged",
        "",
        "lastErrorChanged",
        "messageReceived",
        "QJsonObject",
        "obj",
        "connected",
        "disconnected",
        "onConnected",
        "onDisconnected",
        "onTextMessageReceived",
        "msg",
        "onSslErrors",
        "QList<QSslError>",
        "errors",
        "onErrorOccurred",
        "QAbstractSocket::SocketError",
        "error",
        "scheduleReconnect",
        "heartbeat",
        "connectTo",
        "QUrl",
        "url",
        "nickname",
        "close",
        "sendJson",
        "state",
        "State",
        "lastError",
        "reconnectAttempt",
        "Disconnected",
        "Connecting",
        "Connected",
        "Reconnecting",
        "Failed"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'stateChanged'
        QtMocHelpers::SignalData<void()>(3, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'lastErrorChanged'
        QtMocHelpers::SignalData<void()>(5, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'messageReceived'
        QtMocHelpers::SignalData<void(const QJsonObject &)>(6, 4, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Signal 'connected'
        QtMocHelpers::SignalData<void()>(9, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'disconnected'
        QtMocHelpers::SignalData<void()>(10, 4, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onConnected'
        QtMocHelpers::SlotData<void()>(11, 4, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDisconnected'
        QtMocHelpers::SlotData<void()>(12, 4, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTextMessageReceived'
        QtMocHelpers::SlotData<void(const QString &)>(13, 4, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 14 },
        }}),
        // Slot 'onSslErrors'
        QtMocHelpers::SlotData<void(const QList<QSslError> &)>(15, 4, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 16, 17 },
        }}),
        // Slot 'onErrorOccurred'
        QtMocHelpers::SlotData<void(QAbstractSocket::SocketError)>(18, 4, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 19, 20 },
        }}),
        // Slot 'scheduleReconnect'
        QtMocHelpers::SlotData<void()>(21, 4, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'heartbeat'
        QtMocHelpers::SlotData<void()>(22, 4, QMC::AccessPrivate, QMetaType::Void),
        // Method 'connectTo'
        QtMocHelpers::MethodData<void(const QUrl &, const QString &)>(23, 4, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 24, 25 }, { QMetaType::QString, 26 },
        }}),
        // Method 'close'
        QtMocHelpers::MethodData<void()>(27, 4, QMC::AccessPublic, QMetaType::Void),
        // Method 'sendJson'
        QtMocHelpers::MethodData<void(const QJsonObject &)>(28, 4, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'state'
        QtMocHelpers::PropertyData<enum State>(29, 0x80000000 | 30, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'lastError'
        QtMocHelpers::PropertyData<QString>(31, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
        // property 'reconnectAttempt'
        QtMocHelpers::PropertyData<int>(32, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'State'
        QtMocHelpers::EnumData<enum State>(30, 30, QMC::EnumFlags{}).add({
            {   33, State::Disconnected },
            {   34, State::Connecting },
            {   35, State::Connected },
            {   36, State::Reconnecting },
            {   37, State::Failed },
        }),
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
    });
    return QtMocHelpers::metaObjectData<WsClient, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject WsClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8WsClientE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8WsClientE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8WsClientE_t>.metaTypes,
    nullptr
} };

void WsClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<WsClient *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->stateChanged(); break;
        case 1: _t->lastErrorChanged(); break;
        case 2: _t->messageReceived((*reinterpret_cast<std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 3: _t->connected(); break;
        case 4: _t->disconnected(); break;
        case 5: _t->onConnected(); break;
        case 6: _t->onDisconnected(); break;
        case 7: _t->onTextMessageReceived((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->onSslErrors((*reinterpret_cast<std::add_pointer_t<QList<QSslError>>>(_a[1]))); break;
        case 9: _t->onErrorOccurred((*reinterpret_cast<std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 10: _t->scheduleReconnect(); break;
        case 11: _t->heartbeat(); break;
        case 12: _t->connectTo((*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 13: _t->close(); break;
        case 14: _t->sendJson((*reinterpret_cast<std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QSslError> >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (WsClient::*)()>(_a, &WsClient::stateChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (WsClient::*)()>(_a, &WsClient::lastErrorChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (WsClient::*)(const QJsonObject & )>(_a, &WsClient::messageReceived, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (WsClient::*)()>(_a, &WsClient::connected, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (WsClient::*)()>(_a, &WsClient::disconnected, 4))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<enum State*>(_v) = _t->state(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->lastError(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->reconnectAttempt(); break;
        default: break;
        }
    }
}

const QMetaObject *WsClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WsClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8WsClientE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WsClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void WsClient::stateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void WsClient::lastErrorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void WsClient::messageReceived(const QJsonObject & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void WsClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void WsClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
