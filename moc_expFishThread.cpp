/****************************************************************************
** Meta object code from reading C++ file 'expFishThread.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "FishingHole/expFishThread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'expFishThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_expFishThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   15,   14,   14, 0x05,
      39,   14,   14,   14, 0x05,
      54,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      72,   67,   14,   14, 0x0a,
     111,  107,   14,   14, 0x0a,
     142,  107,   14,   14, 0x0a,
     173,  107,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_expFishThread[] = {
    "expFishThread\0\0,\0sendComm(QString,int)\0"
    "updateWindow()\0naturalEnd()\0,,,,\0"
    "startFish_Exp(int,int,int,int,int)\0"
    ",,,\0startFish_Tr1(int,int,int,int)\0"
    "startFish_Tr2(int,int,int,int)\0"
    "startFish_Tr3(int,int,int,int)\0"
};

void expFishThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        expFishThread *_t = static_cast<expFishThread *>(_o);
        switch (_id) {
        case 0: _t->sendComm((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->updateWindow(); break;
        case 2: _t->naturalEnd(); break;
        case 3: _t->startFish_Exp((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 4: _t->startFish_Tr1((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 5: _t->startFish_Tr2((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 6: _t->startFish_Tr3((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData expFishThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject expFishThread::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_expFishThread,
      qt_meta_data_expFishThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &expFishThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *expFishThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *expFishThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_expFishThread))
        return static_cast<void*>(const_cast< expFishThread*>(this));
    return QObject::qt_metacast(_clname);
}

int expFishThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void expFishThread::sendComm(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void expFishThread::updateWindow()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void expFishThread::naturalEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
