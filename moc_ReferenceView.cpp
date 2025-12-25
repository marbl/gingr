/****************************************************************************
** Meta object code from reading C++ file 'ReferenceView.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/ReferenceView.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReferenceView.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13ReferenceViewE_t {};
} // unnamed namespace

template <> constexpr inline auto ReferenceView::qt_create_metaobjectdata<qt_meta_tag_ZN13ReferenceViewE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ReferenceView",
        "positionChanged",
        "",
        "ungapped",
        "signalMouseWheel",
        "delta",
        "updateSnpsFinished"
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
        // Slot 'updateSnpsFinished'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ReferenceView, qt_meta_tag_ZN13ReferenceViewE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ReferenceView::staticMetaObject = { {
    QMetaObject::SuperData::link<DrawingArea::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ReferenceViewE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ReferenceViewE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13ReferenceViewE_t>.metaTypes,
    nullptr
} };

void ReferenceView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ReferenceView *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->positionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->signalMouseWheel((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->updateSnpsFinished(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ReferenceView::*)(int )>(_a, &ReferenceView::positionChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ReferenceView::*)(int )>(_a, &ReferenceView::signalMouseWheel, 1))
            return;
    }
}

const QMetaObject *ReferenceView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReferenceView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ReferenceViewE_t>.strings))
        return static_cast<void*>(this);
    return DrawingArea::qt_metacast(_clname);
}

int ReferenceView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void ReferenceView::positionChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void ReferenceView::signalMouseWheel(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
QT_WARNING_POP
