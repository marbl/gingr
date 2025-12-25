/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/MainWindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "signalWarning",
        "",
        "message",
        "closeSnps",
        "closeSearch",
        "documentChanged",
        "exportDifferential",
        "const PhylogenyTreeNode*",
        "node",
        "exportSignature",
        "import",
        "fileName",
        "fileNameReference",
        "ImportWindow::FileType",
        "type",
        "menuActionHelp",
        "menuActionAbout",
        "menuExportAlignmentXmfa",
        "menuExportTree",
        "menuExportVariantsMfa",
        "menuExportVariantsVcf",
        "menuHome",
        "menuNew",
        "menuOpen",
        "menuSave",
        "menuSaveAs",
        "menuSnapshot",
        "promptSave",
        "rerootTree",
        "rootNew",
        "rerootTreeMidpoint",
        "setInContextMenu",
        "inContextMenuNew",
        "toggleShowGaps",
        "checked",
        "toggleShowInsertions",
        "toggleShowDeletions",
        "toggleSnps",
        "toggleSearch",
        "toggleRightAlignNodes",
        "toggleRightAlignText",
        "toggleLightColors",
        "saveSnapshot",
        "tree",
        "alignment",
        "setDocumentChanged",
        "setDocumentLoaded",
        "setDocumentUnchanged",
        "setDocumentUnloaded",
        "setNode",
        "setPosition",
        "gapped",
        "toggleSynteny",
        "setAdjustBranchLengths",
        "adjust",
        "setTrackFocus",
        "track",
        "setTrackHover",
        "trackEnd",
        "setTrackListViewFocus",
        "TrackListView*",
        "view",
        "setTrackReference",
        "trackReferenceNew",
        "setTrackZoom",
        "start",
        "end",
        "setWindow",
        "setWindowTarget",
        "unsetTrackListViewFocus",
        "update",
        "updateSnpsFinishedMain",
        "updateSnpsMain",
        "updateSnpsMap",
        "warning",
        "zoomFromMouseWheel",
        "delta"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalWarning'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Slot 'closeSnps'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'closeSearch'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'documentChanged'
        QtMocHelpers::SlotData<bool()>(6, 2, QMC::AccessPublic, QMetaType::Bool),
        // Slot 'exportDifferential'
        QtMocHelpers::SlotData<void(const PhylogenyTreeNode *)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Slot 'exportSignature'
        QtMocHelpers::SlotData<void(const PhylogenyTreeNode *)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Slot 'import'
        QtMocHelpers::SlotData<void(const QString &, const QString &, ImportWindow::FileType)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 12 }, { QMetaType::QString, 13 }, { 0x80000000 | 14, 15 },
        }}),
        // Slot 'menuActionHelp'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'menuActionAbout'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'menuExportAlignmentXmfa'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'menuExportTree'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'menuExportVariantsMfa'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'menuExportVariantsVcf'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'menuHome'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'menuNew'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'menuOpen'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'menuSave'
        QtMocHelpers::SlotData<bool()>(25, 2, QMC::AccessPublic, QMetaType::Bool),
        // Slot 'menuSaveAs'
        QtMocHelpers::SlotData<bool()>(26, 2, QMC::AccessPublic, QMetaType::Bool),
        // Slot 'menuSnapshot'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'promptSave'
        QtMocHelpers::SlotData<bool()>(28, 2, QMC::AccessPublic, QMetaType::Bool),
        // Slot 'rerootTree'
        QtMocHelpers::SlotData<void(const PhylogenyTreeNode *)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 30 },
        }}),
        // Slot 'rerootTree'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'rerootTreeMidpoint'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setInContextMenu'
        QtMocHelpers::SlotData<void(bool)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 33 },
        }}),
        // Slot 'toggleShowGaps'
        QtMocHelpers::SlotData<void(bool)>(34, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 35 },
        }}),
        // Slot 'toggleShowInsertions'
        QtMocHelpers::SlotData<void(bool)>(36, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 35 },
        }}),
        // Slot 'toggleShowDeletions'
        QtMocHelpers::SlotData<void(bool)>(37, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 35 },
        }}),
        // Slot 'toggleSnps'
        QtMocHelpers::SlotData<void(bool)>(38, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 35 },
        }}),
        // Slot 'toggleSearch'
        QtMocHelpers::SlotData<void(bool)>(39, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 35 },
        }}),
        // Slot 'toggleRightAlignNodes'
        QtMocHelpers::SlotData<void(bool)>(40, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 35 },
        }}),
        // Slot 'toggleRightAlignText'
        QtMocHelpers::SlotData<void(bool)>(41, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 35 },
        }}),
        // Slot 'toggleLightColors'
        QtMocHelpers::SlotData<void(bool)>(42, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 35 },
        }}),
        // Slot 'saveSnapshot'
        QtMocHelpers::SlotData<void(const QString &, bool, bool)>(43, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 12 }, { QMetaType::Bool, 44 }, { QMetaType::Bool, 45 },
        }}),
        // Slot 'setDocumentChanged'
        QtMocHelpers::SlotData<void()>(46, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setDocumentLoaded'
        QtMocHelpers::SlotData<void()>(47, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setDocumentUnchanged'
        QtMocHelpers::SlotData<void()>(48, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setDocumentUnloaded'
        QtMocHelpers::SlotData<void()>(49, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setNode'
        QtMocHelpers::SlotData<void(const PhylogenyTreeNode *)>(50, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Slot 'setPosition'
        QtMocHelpers::SlotData<void(int)>(51, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 52 },
        }}),
        // Slot 'toggleSynteny'
        QtMocHelpers::SlotData<void()>(53, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setAdjustBranchLengths'
        QtMocHelpers::SlotData<void(bool)>(54, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 55 },
        }}),
        // Slot 'setTrackFocus'
        QtMocHelpers::SlotData<void(int)>(56, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 57 },
        }}),
        // Slot 'setTrackHover'
        QtMocHelpers::SlotData<void(int, int)>(58, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 57 }, { QMetaType::Int, 59 },
        }}),
        // Slot 'setTrackListViewFocus'
        QtMocHelpers::SlotData<void(TrackListView *)>(60, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 61, 62 },
        }}),
        // Slot 'setTrackReference'
        QtMocHelpers::SlotData<void(int)>(63, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 64 },
        }}),
        // Slot 'setTrackZoom'
        QtMocHelpers::SlotData<void(int, int)>(65, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 66 }, { QMetaType::Int, 67 },
        }}),
        // Slot 'setWindow'
        QtMocHelpers::SlotData<void(int, int)>(68, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 66 }, { QMetaType::Int, 67 },
        }}),
        // Slot 'setWindowTarget'
        QtMocHelpers::SlotData<void(int, int)>(69, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 66 }, { QMetaType::Int, 67 },
        }}),
        // Slot 'unsetTrackListViewFocus'
        QtMocHelpers::SlotData<void(TrackListView *)>(70, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 61, 62 },
        }}),
        // Slot 'update'
        QtMocHelpers::SlotData<void()>(71, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateSnpsFinishedMain'
        QtMocHelpers::SlotData<void()>(72, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateSnpsMain'
        QtMocHelpers::SlotData<void()>(73, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateSnpsMap'
        QtMocHelpers::SlotData<void()>(74, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'warning'
        QtMocHelpers::SlotData<void(const QString &)>(75, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Slot 'zoomFromMouseWheel'
        QtMocHelpers::SlotData<void(int)>(76, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 77 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalWarning((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->closeSnps(); break;
        case 2: _t->closeSearch(); break;
        case 3: { bool _r = _t->documentChanged();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->exportDifferential((*reinterpret_cast< std::add_pointer_t<const PhylogenyTreeNode*>>(_a[1]))); break;
        case 5: _t->exportSignature((*reinterpret_cast< std::add_pointer_t<const PhylogenyTreeNode*>>(_a[1]))); break;
        case 6: _t->import((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<ImportWindow::FileType>>(_a[3]))); break;
        case 7: _t->menuActionHelp(); break;
        case 8: _t->menuActionAbout(); break;
        case 9: _t->menuExportAlignmentXmfa(); break;
        case 10: _t->menuExportTree(); break;
        case 11: _t->menuExportVariantsMfa(); break;
        case 12: _t->menuExportVariantsVcf(); break;
        case 13: _t->menuHome(); break;
        case 14: _t->menuNew(); break;
        case 15: _t->menuOpen(); break;
        case 16: { bool _r = _t->menuSave();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 17: { bool _r = _t->menuSaveAs();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 18: _t->menuSnapshot(); break;
        case 19: { bool _r = _t->promptSave();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 20: _t->rerootTree((*reinterpret_cast< std::add_pointer_t<const PhylogenyTreeNode*>>(_a[1]))); break;
        case 21: _t->rerootTree(); break;
        case 22: _t->rerootTreeMidpoint(); break;
        case 23: _t->setInContextMenu((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 24: _t->toggleShowGaps((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 25: _t->toggleShowInsertions((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 26: _t->toggleShowDeletions((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 27: _t->toggleSnps((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 28: _t->toggleSearch((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 29: _t->toggleRightAlignNodes((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 30: _t->toggleRightAlignText((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 31: _t->toggleLightColors((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 32: _t->saveSnapshot((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        case 33: _t->setDocumentChanged(); break;
        case 34: _t->setDocumentLoaded(); break;
        case 35: _t->setDocumentUnchanged(); break;
        case 36: _t->setDocumentUnloaded(); break;
        case 37: _t->setNode((*reinterpret_cast< std::add_pointer_t<const PhylogenyTreeNode*>>(_a[1]))); break;
        case 38: _t->setPosition((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 39: _t->toggleSynteny(); break;
        case 40: _t->setAdjustBranchLengths((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 41: _t->setTrackFocus((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 42: _t->setTrackHover((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 43: _t->setTrackListViewFocus((*reinterpret_cast< std::add_pointer_t<TrackListView*>>(_a[1]))); break;
        case 44: _t->setTrackReference((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 45: _t->setTrackZoom((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 46: _t->setWindow((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 47: _t->setWindowTarget((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 48: _t->unsetTrackListViewFocus((*reinterpret_cast< std::add_pointer_t<TrackListView*>>(_a[1]))); break;
        case 49: _t->update(); break;
        case 50: _t->updateSnpsFinishedMain(); break;
        case 51: _t->updateSnpsMain(); break;
        case 52: _t->updateSnpsMap(); break;
        case 53: _t->warning((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 54: _t->zoomFromMouseWheel((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 43:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< TrackListView* >(); break;
            }
            break;
        case 48:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< TrackListView* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MainWindow::*)(const QString & )>(_a, &MainWindow::signalWarning, 0))
            return;
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 55)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 55;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 55)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 55;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::signalWarning(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
QT_WARNING_POP
