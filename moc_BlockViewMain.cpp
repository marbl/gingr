/****************************************************************************
** Meta object code from reading C++ file 'BlockViewMain.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/BlockViewMain.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BlockViewMain.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13BlockViewMainE_t {};
} // unnamed namespace

template <> constexpr inline auto BlockViewMain::qt_create_metaobjectdata<qt_meta_tag_ZN13BlockViewMainE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BlockViewMain",
        "signalContextMenu",
        "",
        "inContextMenu",
        "signalMouseWheel",
        "delta",
        "signalTrackReference",
        "trackReferenceNew",
        "signalToggleSynteny",
        "setLcbHover",
        "lcb",
        "offset",
        "setReference",
        "setWindow",
        "start",
        "end",
        "updateSnpsFinished"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalContextMenu'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'signalMouseWheel'
        QtMocHelpers::SignalData<void(int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Signal 'signalTrackReference'
        QtMocHelpers::SignalData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Signal 'signalToggleSynteny'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setLcbHover'
        QtMocHelpers::SlotData<void(int, float)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 10 }, { QMetaType::Float, 11 },
        }}),
        // Slot 'setReference'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setWindow'
        QtMocHelpers::SlotData<void(int, int)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 14 }, { QMetaType::Int, 15 },
        }}),
        // Slot 'updateSnpsFinished'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<BlockViewMain, qt_meta_tag_ZN13BlockViewMainE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject BlockViewMain::staticMetaObject = { {
    QMetaObject::SuperData::link<BlockView::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13BlockViewMainE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13BlockViewMainE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13BlockViewMainE_t>.metaTypes,
    nullptr
} };

void BlockViewMain::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BlockViewMain *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalContextMenu((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->signalMouseWheel((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->signalTrackReference((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->signalToggleSynteny(); break;
        case 4: _t->setLcbHover((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[2]))); break;
        case 5: _t->setReference(); break;
        case 6: _t->setWindow((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 7: _t->updateSnpsFinished(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (BlockViewMain::*)(bool )>(_a, &BlockViewMain::signalContextMenu, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (BlockViewMain::*)(int )>(_a, &BlockViewMain::signalMouseWheel, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (BlockViewMain::*)(int )>(_a, &BlockViewMain::signalTrackReference, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (BlockViewMain::*)()>(_a, &BlockViewMain::signalToggleSynteny, 3))
            return;
    }
}

const QMetaObject *BlockViewMain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BlockViewMain::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13BlockViewMainE_t>.strings))
        return static_cast<void*>(this);
    return BlockView::qt_metacast(_clname);
}

int BlockViewMain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BlockView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void BlockViewMain::signalContextMenu(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void BlockViewMain::signalMouseWheel(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void BlockViewMain::signalTrackReference(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void BlockViewMain::signalToggleSynteny()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
