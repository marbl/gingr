/****************************************************************************
** Meta object code from reading C++ file 'PhylogenyTreeView.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/PhylogenyTreeView.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PhylogenyTreeView.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN17PhylogenyTreeViewE_t {};
} // unnamed namespace

template <> constexpr inline auto PhylogenyTreeView::qt_create_metaobjectdata<qt_meta_tag_ZN17PhylogenyTreeViewE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PhylogenyTreeView",
        "signalNodeHover",
        "",
        "const PhylogenyTreeNode*",
        "node",
        "signalTrackZoom",
        "start",
        "end"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalNodeHover'
        QtMocHelpers::SignalData<void(const PhylogenyTreeNode *)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'signalTrackZoom'
        QtMocHelpers::SignalData<void(int, int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::Int, 7 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PhylogenyTreeView, qt_meta_tag_ZN17PhylogenyTreeViewE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PhylogenyTreeView::staticMetaObject = { {
    QMetaObject::SuperData::link<TrackListView::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17PhylogenyTreeViewE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17PhylogenyTreeViewE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17PhylogenyTreeViewE_t>.metaTypes,
    nullptr
} };

void PhylogenyTreeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PhylogenyTreeView *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalNodeHover((*reinterpret_cast< std::add_pointer_t<const PhylogenyTreeNode*>>(_a[1]))); break;
        case 1: _t->signalTrackZoom((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PhylogenyTreeView::*)(const PhylogenyTreeNode * )>(_a, &PhylogenyTreeView::signalNodeHover, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PhylogenyTreeView::*)(int , int )>(_a, &PhylogenyTreeView::signalTrackZoom, 1))
            return;
    }
}

const QMetaObject *PhylogenyTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PhylogenyTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17PhylogenyTreeViewE_t>.strings))
        return static_cast<void*>(this);
    return TrackListView::qt_metacast(_clname);
}

int PhylogenyTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TrackListView::qt_metacall(_c, _id, _a);
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
void PhylogenyTreeView::signalNodeHover(const PhylogenyTreeNode * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void PhylogenyTreeView::signalTrackZoom(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}
QT_WARNING_POP
