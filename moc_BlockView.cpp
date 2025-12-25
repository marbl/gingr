/****************************************************************************
** Meta object code from reading C++ file 'BlockView.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/BlockView.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BlockView.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9BlockViewE_t {};
} // unnamed namespace

template <> constexpr inline auto BlockView::qt_create_metaobjectdata<qt_meta_tag_ZN9BlockViewE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BlockView",
        "positionChanged",
        "",
        "ungapped",
        "signalLcbHoverChange",
        "lcb",
        "offset",
        "signalUpdateSnps",
        "windowChanged",
        "start",
        "end",
        "updateSnpsFinished",
        "updateSnpsNeeded"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'positionChanged'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'signalLcbHoverChange'
        QtMocHelpers::SignalData<void(int, float)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 }, { QMetaType::Float, 6 },
        }}),
        // Signal 'signalUpdateSnps'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'windowChanged'
        QtMocHelpers::SignalData<void(int, int)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 }, { QMetaType::Int, 10 },
        }}),
        // Slot 'updateSnpsFinished'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateSnpsNeeded'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<BlockView, qt_meta_tag_ZN9BlockViewE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject BlockView::staticMetaObject = { {
    QMetaObject::SuperData::link<TrackListView::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9BlockViewE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9BlockViewE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9BlockViewE_t>.metaTypes,
    nullptr
} };

void BlockView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BlockView *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->positionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->signalLcbHoverChange((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[2]))); break;
        case 2: _t->signalUpdateSnps(); break;
        case 3: _t->windowChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->updateSnpsFinished(); break;
        case 5: _t->updateSnpsNeeded(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (BlockView::*)(int )>(_a, &BlockView::positionChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (BlockView::*)(int , float )>(_a, &BlockView::signalLcbHoverChange, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (BlockView::*)()>(_a, &BlockView::signalUpdateSnps, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (BlockView::*)(int , int )>(_a, &BlockView::windowChanged, 3))
            return;
    }
}

const QMetaObject *BlockView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BlockView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9BlockViewE_t>.strings))
        return static_cast<void*>(this);
    return TrackListView::qt_metacast(_clname);
}

int BlockView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TrackListView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void BlockView::positionChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void BlockView::signalLcbHoverChange(int _t1, float _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void BlockView::signalUpdateSnps()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void BlockView::windowChanged(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}
QT_WARNING_POP
