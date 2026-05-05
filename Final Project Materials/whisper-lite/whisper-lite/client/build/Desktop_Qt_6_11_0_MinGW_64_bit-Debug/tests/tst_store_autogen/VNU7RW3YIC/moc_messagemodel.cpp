/****************************************************************************
** Meta object code from reading C++ file 'messagemodel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/messagemodel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'messagemodel.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12MessageModelE_t {};
} // unnamed namespace

template <> constexpr inline auto MessageModel::qt_create_metaobjectdata<qt_meta_tag_ZN12MessageModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MessageModel",
        "activePeerChanged",
        "",
        "countChanged",
        "activePeer",
        "count"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'activePeerChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'countChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'activePeer'
        QtMocHelpers::PropertyData<QString>(4, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'count'
        QtMocHelpers::PropertyData<int>(5, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MessageModel, qt_meta_tag_ZN12MessageModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MessageModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12MessageModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12MessageModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12MessageModelE_t>.metaTypes,
    nullptr
} };

void MessageModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MessageModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->activePeerChanged(); break;
        case 1: _t->countChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MessageModel::*)()>(_a, &MessageModel::activePeerChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (MessageModel::*)()>(_a, &MessageModel::countChanged, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->activePeer(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->rowCount(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setActivePeer(*reinterpret_cast<QString*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *MessageModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MessageModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12MessageModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int MessageModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void MessageModel::activePeerChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MessageModel::countChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
