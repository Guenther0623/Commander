/****************************************************************************
** Meta object code from reading C++ file 'circleMaze.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CircleMaze/circleMaze.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'circleMaze.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CircleMaze[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   11,   11,   11, 0x08,
      77,   11,   11,   11, 0x08,
     104,   11,   11,   11, 0x08,
     132,   11,   11,   11, 0x08,
     150,   11,   11,   11, 0x08,
     173,   11,   11,   11, 0x08,
     189,   11,   11,   11, 0x08,
     209,   11,   11,   11, 0x08,
     229,   11,   11,   11, 0x08,
     244,   11,  239,   11, 0x08,
     263,  261,   11,   11, 0x08,
     283,   11,   11,   11, 0x08,
     294,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CircleMaze[] = {
    "CircleMaze\0\0,,,,\0StartCircle(int,int,int,int,int)\0"
    "updateValue_startZone(int)\0"
    "updateValue_zones(QString)\0"
    "updateValue_trials(QString)\0"
    "experimentSTART()\0requestExperimentEND()\0"
    "experimentEND()\0ardPortChanged(int)\0"
    "ardBaudChanged(int)\0testArd()\0bool\0"
    "isArdConnected()\0,\0addLog(QString,int)\0"
    "clearLog()\0tickTock()\0"
};

void CircleMaze::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CircleMaze *_t = static_cast<CircleMaze *>(_o);
        switch (_id) {
        case 0: _t->StartCircle((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 1: _t->updateValue_startZone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->updateValue_zones((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->updateValue_trials((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->experimentSTART(); break;
        case 5: _t->requestExperimentEND(); break;
        case 6: _t->experimentEND(); break;
        case 7: _t->ardPortChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->ardBaudChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->testArd(); break;
        case 10: { bool _r = _t->isArdConnected();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: _t->addLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->clearLog(); break;
        case 13: _t->tickTock(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CircleMaze::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CircleMaze::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CircleMaze,
      qt_meta_data_CircleMaze, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CircleMaze::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CircleMaze::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CircleMaze::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CircleMaze))
        return static_cast<void*>(const_cast< CircleMaze*>(this));
    return QWidget::qt_metacast(_clname);
}

int CircleMaze::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void CircleMaze::StartCircle(int _t1, int _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
