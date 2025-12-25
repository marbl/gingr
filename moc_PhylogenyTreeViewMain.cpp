/****************************************************************************
** Meta object code from reading C++ file 'PhylogenyTreeViewMain.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/PhylogenyTreeViewMain.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PhylogenyTreeViewMain.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN21PhylogenyTreeViewMainE_t {};
} // unnamed namespace

template <> constexpr inline auto PhylogenyTreeViewMain::qt_create_metaobjectdata<qt_meta_tag_ZN21PhylogenyTreeViewMainE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PhylogenyTreeViewMain",
        "signalContextMenu",
        "",
        "inContextMenu",
        "signalExportDifferential",
        "const PhylogenyTreeNode*",
        "node",
        "signalExportSignature",
        "signalFocusNode",
        "zoomIn",
        "signalReroot",
        "rootNew",
        "signalSearchResults",
        "results",
        "reroot",
        "exportDifferential",
        "exportSignature",
        "search",
        "string",
        "matchCase"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalContextMenu'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'signalExportDifferential'
        QtMocHelpers::SignalData<void(const PhylogenyTreeNode *) const>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Signal 'signalExportSignature'
        QtMocHelpers::SignalData<void(const PhylogenyTreeNode *) const>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Signal 'signalFocusNode'
        QtMocHelpers::SignalData<void(const PhylogenyTreeNode *, bool) const>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 }, { QMetaType::Bool, 9 },
        }}),
        // Signal 'signalReroot'
        QtMocHelpers::SignalData<void(const PhylogenyTreeNode *) const>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 11 },
        }}),
        // Signal 'signalSearchResults'
        QtMocHelpers::SignalData<void(int) const>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Slot 'reroot'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'exportDifferential'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'exportSignature'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'search'
        QtMocHelpers::SlotData<void(const QString &, bool)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 18 }, { QMetaType::Bool, 19 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PhylogenyTreeViewMain, qt_meta_tag_ZN21PhylogenyTreeViewMainE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PhylogenyTreeViewMain::staticMetaObject = { {
    QMetaObject::SuperData::link<PhylogenyTreeView::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21PhylogenyTreeViewMainE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21PhylogenyTreeViewMainE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN21PhylogenyTreeViewMainE_t>.metaTypes,
    nullptr
} };

void PhylogenyTreeViewMain::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PhylogenyTreeViewMain *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalContextMenu((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->signalExportDifferential((*reinterpret_cast< std::add_pointer_t<const PhylogenyTreeNode*>>(_a[1]))); break;
        case 2: _t->signalExportSignature((*reinterpret_cast< std::add_pointer_t<const PhylogenyTreeNode*>>(_a[1]))); break;
        case 3: _t->signalFocusNode((*reinterpret_cast< std::add_pointer_t<const PhylogenyTreeNode*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 4: _t->signalReroot((*reinterpret_cast< std::add_pointer_t<const PhylogenyTreeNode*>>(_a[1]))); break;
        case 5: _t->signalSearchResults((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->reroot(); break;
        case 7: _t->exportDifferential(); break;
        case 8: _t->exportSignature(); break;
        case 9: _t->search((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PhylogenyTreeViewMain::*)(bool )>(_a, &PhylogenyTreeViewMain::signalContextMenu, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PhylogenyTreeViewMain::*)(const PhylogenyTreeNode * ) const>(_a, &PhylogenyTreeViewMain::signalExportDifferential, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (PhylogenyTreeViewMain::*)(const PhylogenyTreeNode * ) const>(_a, &PhylogenyTreeViewMain::signalExportSignature, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (PhylogenyTreeViewMain::*)(const PhylogenyTreeNode * , bool ) const>(_a, &PhylogenyTreeViewMain::signalFocusNode, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (PhylogenyTreeViewMain::*)(const PhylogenyTreeNode * ) const>(_a, &PhylogenyTreeViewMain::signalReroot, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (PhylogenyTreeViewMain::*)(int ) const>(_a, &PhylogenyTreeViewMain::signalSearchResults, 5))
            return;
    }
}

const QMetaObject *PhylogenyTreeViewMain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PhylogenyTreeViewMain::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21PhylogenyTreeViewMainE_t>.strings))
        return static_cast<void*>(this);
    return PhylogenyTreeView::qt_metacast(_clname);
}

int PhylogenyTreeViewMain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PhylogenyTreeView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void PhylogenyTreeViewMain::signalContextMenu(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void PhylogenyTreeViewMain::signalExportDifferential(const PhylogenyTreeNode * _t1)const
{
    QMetaObject::activate<void>(const_cast< PhylogenyTreeViewMain *>(this), &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void PhylogenyTreeViewMain::signalExportSignature(const PhylogenyTreeNode * _t1)const
{
    QMetaObject::activate<void>(const_cast< PhylogenyTreeViewMain *>(this), &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void PhylogenyTreeViewMain::signalFocusNode(const PhylogenyTreeNode * _t1, bool _t2)const
{
    QMetaObject::activate<void>(const_cast< PhylogenyTreeViewMain *>(this), &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void PhylogenyTreeViewMain::signalReroot(const PhylogenyTreeNode * _t1)const
{
    QMetaObject::activate<void>(const_cast< PhylogenyTreeViewMain *>(this), &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void PhylogenyTreeViewMain::signalSearchResults(int _t1)const
{
    QMetaObject::activate<void>(const_cast< PhylogenyTreeViewMain *>(this), &staticMetaObject, 5, nullptr, _t1);
}
QT_WARNING_POP
