/****************************************************************************
** Meta object code from reading C++ file 'PhylogenyTreeView.h'
**
** Created: Mon Apr 8 14:23:16 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/PhylogenyTreeView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PhylogenyTreeView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PhylogenyTreeView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   19,   18,   18, 0x05,
      72,   62,   18,   18, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_PhylogenyTreeView[] = {
    "PhylogenyTreeView\0\0node\0"
    "signalNodeHover(const PhylogenyNode*)\0"
    "start,end\0signalTrackZoom(int,int)\0"
};

void PhylogenyTreeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PhylogenyTreeView *_t = static_cast<PhylogenyTreeView *>(_o);
        switch (_id) {
        case 0: _t->signalNodeHover((*reinterpret_cast< const PhylogenyNode*(*)>(_a[1]))); break;
        case 1: _t->signalTrackZoom((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PhylogenyTreeView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PhylogenyTreeView::staticMetaObject = {
    { &TrackListView::staticMetaObject, qt_meta_stringdata_PhylogenyTreeView,
      qt_meta_data_PhylogenyTreeView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PhylogenyTreeView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PhylogenyTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PhylogenyTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PhylogenyTreeView))
        return static_cast<void*>(const_cast< PhylogenyTreeView*>(this));
    return TrackListView::qt_metacast(_clname);
}

int PhylogenyTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TrackListView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void PhylogenyTreeView::signalNodeHover(const PhylogenyNode * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PhylogenyTreeView::signalTrackZoom(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
