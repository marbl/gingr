/****************************************************************************
** Meta object code from reading C++ file 'BlockView.h'
**
** Created: Fri Apr 5 16:57:09 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/BlockView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BlockView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BlockView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   11,   10,   10, 0x05,
      74,   63,   10,   10, 0x05,
     116,  106,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     139,   63,   10,   10, 0x0a,
     162,   10,   10,   10, 0x0a,
     183,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_BlockView[] = {
    "BlockView\0\0ungapped,gapped,offset\0"
    "positionChanged(int,int,int)\0lcb,offset\0"
    "signalLcbHoverChange(int,float)\0"
    "start,end\0windowChanged(int,int)\0"
    "setLcbHover(int,float)\0updateSnpsFinished()\0"
    "updateSnpsNeeded()\0"
};

void BlockView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BlockView *_t = static_cast<BlockView *>(_o);
        switch (_id) {
        case 0: _t->positionChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->signalLcbHoverChange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 2: _t->windowChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->setLcbHover((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 4: _t->updateSnpsFinished(); break;
        case 5: _t->updateSnpsNeeded(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BlockView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BlockView::staticMetaObject = {
    { &TrackListView::staticMetaObject, qt_meta_stringdata_BlockView,
      qt_meta_data_BlockView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BlockView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BlockView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BlockView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BlockView))
        return static_cast<void*>(const_cast< BlockView*>(this));
    return TrackListView::qt_metacast(_clname);
}

int BlockView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TrackListView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void BlockView::positionChanged(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BlockView::signalLcbHoverChange(int _t1, float _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BlockView::windowChanged(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
