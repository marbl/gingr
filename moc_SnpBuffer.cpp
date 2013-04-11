/****************************************************************************
** Meta object code from reading C++ file 'SnpBuffer.h'
**
** Created: Fri Apr 5 16:57:09 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/SnpBuffer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SnpBuffer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SnpBuffer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      21,   10,   10,   10, 0x0a,
      42,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SnpBuffer[] = {
    "SnpBuffer\0\0updated()\0threadError(QString)\0"
    "updateFinished()\0"
};

void SnpBuffer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SnpBuffer *_t = static_cast<SnpBuffer *>(_o);
        switch (_id) {
        case 0: _t->updated(); break;
        case 1: _t->threadError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->updateFinished(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SnpBuffer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SnpBuffer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SnpBuffer,
      qt_meta_data_SnpBuffer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SnpBuffer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SnpBuffer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SnpBuffer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SnpBuffer))
        return static_cast<void*>(const_cast< SnpBuffer*>(this));
    return QObject::qt_metacast(_clname);
}

int SnpBuffer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void SnpBuffer::updated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
