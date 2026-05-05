/****************************************************************************
** Meta object code from reading C++ file 'usermodel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/usermodel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'usermodel.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9UserModelE_t {};
} // unnamed namespace

template <> constexpr inline auto UserModel::qt_create_metaobjectdata<qt_meta_tag_ZN9UserModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "UserModel",
        "QML.Element",
        "auto",
        "countChanged",
        "",
        "userAdded",
        "nick",
        "pubKeyB64",
        "pubKeyOf",
        "isOnline",
        "count"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'countChanged'
        QtMocHelpers::SignalData<void()>(3, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'userAdded'
        QtMocHelpers::SignalData<void(const QString &, const QByteArray &)>(5, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 }, { QMetaType::QByteArray, 7 },
        }}),
        // Method 'pubKeyOf'
        QtMocHelpers::MethodData<QByteArray(const QString &) const>(8, 4, QMC::AccessPublic, QMetaType::QByteArray, {{
            { QMetaType::QString, 6 },
        }}),
        // Method 'isOnline'
        QtMocHelpers::MethodData<bool(const QString &) const>(9, 4, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 6 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'count'
        QtMocHelpers::PropertyData<int>(10, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
    });
    return QtMocHelpers::metaObjectData<UserModel, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject UserModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UserModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UserModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9UserModelE_t>.metaTypes,
    nullptr
} };

void UserModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<UserModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->countChanged(); break;
        case 1: _t->userAdded((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QByteArray>>(_a[2]))); break;
        case 2: { QByteArray _r = _t->pubKeyOf((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QByteArray*>(_a[0]) = std::move(_r); }  break;
        case 3: { bool _r = _t->isOnline((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (UserModel::*)()>(_a, &UserModel::countChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (UserModel::*)(const QString & , const QByteArray & )>(_a, &UserModel::userAdded, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->rowCount(); break;
        default: break;
        }
    }
}

const QMetaObject *UserModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UserModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int UserModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void UserModel::countChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void UserModel::userAdded(const QString & _t1, const QByteArray & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}
QT_WARNING_POP
