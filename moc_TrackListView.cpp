/****************************************************************************
** Meta object code from reading C++ file 'TrackListView.h'
**
** Created: Tue Apr 2 18:23:43 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/TrackListView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TrackListView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TrackListView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   15,   14,   14, 0x05,
      54,   48,   14,   14, 0x05,
      97,   82,   14,   14, 0x05,
     129,   15,   14,   14, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_TrackListView[] = {
    "TrackListView\0\0view\0signalFocus(TrackListView*)\0"
    "track\0signalTrackFocusChange(int)\0"
    "track,trackEnd\0signalTrackHoverChange(int,int)\0"
    "signalUnfocus(TrackListView*)\0"
};

void TrackListView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TrackListView *_t = static_cast<TrackListView *>(_o);
        switch (_id) {
        case 0: _t->signalFocus((*reinterpret_cast< TrackListView*(*)>(_a[1]))); break;
        case 1: _t->signalTrackFocusChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->signalTrackHoverChange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->signalUnfocus((*reinterpret_cast< TrackListView*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TrackListView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TrackListView::staticMetaObject = {
    { &DrawingArea::staticMetaObject, qt_meta_stringdata_TrackListView,
      qt_meta_data_TrackListView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TrackListView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TrackListView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TrackListView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TrackListView))
        return static_cast<void*>(const_cast< TrackListView*>(this));
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
    return _id;
}

// SIGNAL 0
void TrackListView::signalFocus(TrackListView * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TrackListView::signalTrackFocusChange(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TrackListView::signalTrackHoverChange(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TrackListView::signalUnfocus(TrackListView * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
