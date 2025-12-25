/****************************************************************************
** Meta object code from reading C++ file 'BlockViewMap.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/BlockViewMap.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BlockViewMap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.3. It"
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
struct qt_meta_tag_ZN12BlockViewMapE_t {};
} // unnamed namespace

template <> constexpr inline auto BlockViewMap::qt_create_metaobjectdata<qt_meta_tag_ZN12BlockViewMapE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BlockViewMap",
        "signalMouseWheel",
        "",
        "delta",
        "signalWindowChanged",
        "start",
        "end"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalMouseWheel'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'signalWindowChanged'
        QtMocHelpers::SignalData<void(int, int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 }, { QMetaType::Int, 6 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<BlockViewMap, qt_meta_tag_ZN12BlockViewMapE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject BlockViewMap::staticMetaObject = { {
    QMetaObject::SuperData::link<BlockView::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12BlockViewMapE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12BlockViewMapE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12BlockViewMapE_t>.metaTypes,
    nullptr
} };

void BlockViewMap::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BlockViewMap *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalMouseWheel((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->signalWindowChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (BlockViewMap::*)(int )>(_a, &BlockViewMap::signalMouseWheel, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (BlockViewMap::*)(int , int )>(_a, &BlockViewMap::signalWindowChanged, 1))
            return;
    }
}

const QMetaObject *BlockViewMap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BlockViewMap::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12BlockViewMapE_t>.strings))
        return static_cast<void*>(this);
    return BlockView::qt_metacast(_clname);
}

int BlockViewMap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BlockView::qt_metacall(_c, _id, _a);
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
    return _id;
}

// SIGNAL 0
void BlockViewMap::signalMouseWheel(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void BlockViewMap::signalWindowChanged(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}
QT_WARNING_POP
