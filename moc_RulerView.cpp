/****************************************************************************
** Meta object code from reading C++ file 'RulerView.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/RulerView.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RulerView.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9RulerViewE_t {};
} // unnamed namespace

template <> constexpr inline auto RulerView::qt_create_metaobjectdata<qt_meta_tag_ZN9RulerViewE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "RulerView",
        "positionChanged",
        "",
        "ungapped",
        "signalMouseWheel",
        "delta",
        "signalWindowTarget",
        "start",
        "end"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'positionChanged'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'signalMouseWheel'
        QtMocHelpers::SignalData<void(int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Signal 'signalWindowTarget'
        QtMocHelpers::SignalData<void(int, int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 }, { QMetaType::Int, 8 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<RulerView, qt_meta_tag_ZN9RulerViewE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject RulerView::staticMetaObject = { {
    QMetaObject::SuperData::link<DrawingArea::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9RulerViewE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9RulerViewE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9RulerViewE_t>.metaTypes,
    nullptr
} };

void RulerView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<RulerView *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->positionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->signalMouseWheel((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->signalWindowTarget((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (RulerView::*)(int )>(_a, &RulerView::positionChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (RulerView::*)(int )>(_a, &RulerView::signalMouseWheel, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (RulerView::*)(int , int )>(_a, &RulerView::signalWindowTarget, 2))
            return;
    }
}

const QMetaObject *RulerView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RulerView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9RulerViewE_t>.strings))
        return static_cast<void*>(this);
    return DrawingArea::qt_metacast(_clname);
}

int RulerView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DrawingArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void RulerView::positionChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void RulerView::signalMouseWheel(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void RulerView::signalWindowTarget(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}
QT_WARNING_POP
