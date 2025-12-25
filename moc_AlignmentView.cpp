/****************************************************************************
** Meta object code from reading C++ file 'AlignmentView.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/AlignmentView.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AlignmentView.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13AlignmentViewE_t {};
} // unnamed namespace

template <> constexpr inline auto AlignmentView::qt_create_metaobjectdata<qt_meta_tag_ZN13AlignmentViewE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AlignmentView",
        "signalLcbHoverChange",
        "",
        "lcb",
        "offset",
        "setLcbHover"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalLcbHoverChange'
        QtMocHelpers::SignalData<void(int, float)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Float, 4 },
        }}),
        // Slot 'setLcbHover'
        QtMocHelpers::SlotData<void(int, float)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Float, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AlignmentView, qt_meta_tag_ZN13AlignmentViewE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AlignmentView::staticMetaObject = { {
    QMetaObject::SuperData::link<TrackListView::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AlignmentViewE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AlignmentViewE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13AlignmentViewE_t>.metaTypes,
    nullptr
} };

void AlignmentView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AlignmentView *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalLcbHoverChange((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[2]))); break;
        case 1: _t->setLcbHover((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (AlignmentView::*)(int , float )>(_a, &AlignmentView::signalLcbHoverChange, 0))
            return;
    }
}

const QMetaObject *AlignmentView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AlignmentView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AlignmentViewE_t>.strings))
        return static_cast<void*>(this);
    return TrackListView::qt_metacast(_clname);
}

int AlignmentView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void AlignmentView::signalLcbHoverChange(int _t1, float _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}
QT_WARNING_POP
