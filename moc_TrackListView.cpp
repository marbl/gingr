/****************************************************************************
** Meta object code from reading C++ file 'TrackListView.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/TrackListView.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TrackListView.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13TrackListViewE_t {};
} // unnamed namespace

template <> constexpr inline auto TrackListView::qt_create_metaobjectdata<qt_meta_tag_ZN13TrackListViewE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TrackListView",
        "signalFocus",
        "",
        "TrackListView*",
        "view",
        "signalTrackFocusChange",
        "track",
        "signalTrackHoverChange",
        "trackEnd",
        "signalUnfocus"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalFocus'
        QtMocHelpers::SignalData<void(TrackListView *)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'signalTrackFocusChange'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Signal 'signalTrackHoverChange'
        QtMocHelpers::SignalData<void(int, int)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::Int, 8 },
        }}),
        // Signal 'signalUnfocus'
        QtMocHelpers::SignalData<void(TrackListView *)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TrackListView, qt_meta_tag_ZN13TrackListViewE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TrackListView::staticMetaObject = { {
    QMetaObject::SuperData::link<DrawingArea::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13TrackListViewE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13TrackListViewE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13TrackListViewE_t>.metaTypes,
    nullptr
} };

void TrackListView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TrackListView *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalFocus((*reinterpret_cast< std::add_pointer_t<TrackListView*>>(_a[1]))); break;
        case 1: _t->signalTrackFocusChange((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->signalTrackHoverChange((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->signalUnfocus((*reinterpret_cast< std::add_pointer_t<TrackListView*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< TrackListView* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< TrackListView* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TrackListView::*)(TrackListView * )>(_a, &TrackListView::signalFocus, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TrackListView::*)(int )>(_a, &TrackListView::signalTrackFocusChange, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TrackListView::*)(int , int )>(_a, &TrackListView::signalTrackHoverChange, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (TrackListView::*)(TrackListView * )>(_a, &TrackListView::signalUnfocus, 3))
            return;
    }
}

const QMetaObject *TrackListView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TrackListView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13TrackListViewE_t>.strings))
        return static_cast<void*>(this);
    return DrawingArea::qt_metacast(_clname);
}

int TrackListView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DrawingArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void TrackListView::signalFocus(TrackListView * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void TrackListView::signalTrackFocusChange(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void TrackListView::signalTrackHoverChange(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void TrackListView::signalUnfocus(TrackListView * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
