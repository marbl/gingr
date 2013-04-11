/****************************************************************************
** Meta object code from reading C++ file 'FilterControl.h'
**
** Created: Fri Apr 5 16:57:09 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/FilterControl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FilterControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FilterControl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      35,   32,   14,   14, 0x0a,
      62,   56,   14,   14, 0x0a,
      87,   32,   14,   14, 0x0a,
     110,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FilterControl[] = {
    "FilterControl\0\0filtersChanged()\0id\0"
    "checkBoxChanged(int)\0state\0"
    "checkBoxPassChanged(int)\0"
    "showButtonChanged(int)\0scaleButtonClicked(bool)\0"
};

void FilterControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FilterControl *_t = static_cast<FilterControl *>(_o);
        switch (_id) {
        case 0: _t->filtersChanged(); break;
        case 1: _t->checkBoxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->checkBoxPassChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->showButtonChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->scaleButtonClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FilterControl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FilterControl::staticMetaObject = {
    { &OptionPanel::staticMetaObject, qt_meta_stringdata_FilterControl,
      qt_meta_data_FilterControl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FilterControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FilterControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FilterControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FilterControl))
        return static_cast<void*>(const_cast< FilterControl*>(this));
    return OptionPanel::qt_metacast(_clname);
}

int FilterControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = OptionPanel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void FilterControl::filtersChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
