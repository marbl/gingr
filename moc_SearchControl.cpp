/****************************************************************************
** Meta object code from reading C++ file 'SearchControl.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/SearchControl.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SearchControl.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13SearchControlE_t {};
} // unnamed namespace

template <> constexpr inline auto SearchControl::qt_create_metaobjectdata<qt_meta_tag_ZN13SearchControlE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SearchControl",
        "signalSearchChanged",
        "",
        "text",
        "matchCase",
        "clear",
        "matchCaseChaged",
        "state",
        "resultsChangedTracks",
        "results",
        "resultsChangedAnnotations",
        "textChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalSearchChanged'
        QtMocHelpers::SignalData<void(const QString &, bool) const>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::Bool, 4 },
        }}),
        // Slot 'clear'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'matchCaseChaged'
        QtMocHelpers::SlotData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Slot 'resultsChangedTracks'
        QtMocHelpers::SlotData<void(int)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'resultsChangedAnnotations'
        QtMocHelpers::SlotData<void(int)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 2 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SearchControl, qt_meta_tag_ZN13SearchControlE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SearchControl::staticMetaObject = { {
    QMetaObject::SuperData::link<OptionPanel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SearchControlE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SearchControlE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13SearchControlE_t>.metaTypes,
    nullptr
} };

void SearchControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SearchControl *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalSearchChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 1: _t->clear(); break;
        case 2: _t->matchCaseChaged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->resultsChangedTracks((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->resultsChangedAnnotations((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SearchControl::*)(const QString & , bool ) const>(_a, &SearchControl::signalSearchChanged, 0))
            return;
    }
}

const QMetaObject *SearchControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SearchControl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SearchControlE_t>.strings))
        return static_cast<void*>(this);
    return OptionPanel::qt_metacast(_clname);
}

int SearchControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = OptionPanel::qt_metacall(_c, _id, _a);
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
void SearchControl::signalSearchChanged(const QString & _t1, bool _t2)const
{
    QMetaObject::activate<void>(const_cast< SearchControl *>(this), &staticMetaObject, 0, nullptr, _t1, _t2);
}
QT_WARNING_POP
