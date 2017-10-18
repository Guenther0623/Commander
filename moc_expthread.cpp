/****************************************************************************
** Meta object code from reading C++ file 'expthread.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "expthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'expthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_expThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,   35,   10,   10, 0x0a,
      76,   10,   10,   10, 0x0a,
      88,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_expThread[] = {
    "expThread\0\0,\0sendComm(QString,int)\0"
    ",,,,,\0startDNMS(int,int,int,int,int,int)\0"
    "startRand()\0startFish()\0"
};

void expThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        expThread *_t = static_cast<expThread *>(_o);
        switch (_id) {
        case 0: _t->sendComm((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->startDNMS((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 2: _t->startRand(); break;
        case 3: _t->startFish(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData expThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject expThread::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_expThread,
      qt_meta_data_expThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &expThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *expThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *expThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_expThread))
        return static_cast<void*>(const_cast< expThread*>(this));
    return QObject::qt_metacast(_clname);
}

int expThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void expThread::sendComm(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
