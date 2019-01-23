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
      35,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x05,
      55,   12,   11,   11, 0x05,
      91,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     127,   11,   11,   11, 0x08,
     150,   11,   11,   11, 0x08,
     176,   11,   11,   11, 0x08,
     201,   11,   11,   11, 0x08,
     225,   11,   11,   11, 0x08,
     252,   11,   11,   11, 0x08,
     278,   11,   11,   11, 0x08,
     303,   11,   11,   11, 0x08,
     327,   11,   11,   11, 0x08,
     354,   11,   11,   11, 0x08,
     380,   11,   11,   11, 0x08,
     405,   11,   11,   11, 0x08,
     427,   11,   11,   11, 0x08,
     448,   11,   11,   11, 0x08,
     469,   11,   11,   11, 0x08,
     490,   11,   11,   11, 0x08,
     513,   11,   11,   11, 0x08,
     529,   11,   11,   11, 0x08,
     549,   11,   11,   11, 0x08,
     569,   11,   11,   11, 0x08,
     584,   11,  579,   11, 0x08,
     603,  601,   11,   11, 0x08,
     623,   11,   11,   11, 0x08,
     634,   11,   11,   11, 0x08,
     645,   11,   11,   11, 0x08,
     670,   11,   11,   11, 0x08,
     695,   11,   11,   11, 0x08,
     720,   11,   11,   11, 0x08,
     745,   11,   11,   11, 0x08,
     770,   11,   11,   11, 0x08,
     795,   11,   11,   11, 0x08,
     820,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CircleMaze[] = {
    "CircleMaze\0\0,,,,\0StartCircle_Exp(int,int,int*,int,int)\0"
    "StartCircle_T1(int,int,int,int,int)\0"
    "StartCircle_T2(int,int,int,int,int)\0"
    "ratChanged_E1(QString)\0trialsChanged_E1(QString)\0"
    "sZoneChanged_E1(QString)\0"
    "ratChanged_TP1(QString)\0"
    "trialsChanged_TP1(QString)\0"
    "sZoneChanged_TP1(QString)\0"
    "sITIChanged_TP1(QString)\0"
    "ratChanged_TP2(QString)\0"
    "trialsChanged_TP2(QString)\0"
    "sZoneChanged_TP2(QString)\0"
    "sITIChanged_TP2(QString)\0delaySpinChanged(int)\0"
    "experimentSTART_E1()\0experimentSTART_T1()\0"
    "experimentSTART_T2()\0requestExperimentEND()\0"
    "experimentEND()\0ardPortChanged(int)\0"
    "ardBaudChanged(int)\0testArd()\0bool\0"
    "isArdConnected()\0,\0addLog(QString,int)\0"
    "clearLog()\0tickTock()\0circleL1Changed(QString)\0"
    "circleL2Changed(QString)\0"
    "circleL3Changed(QString)\0"
    "circleL4Changed(QString)\0"
    "circleL5Changed(QString)\0"
    "circleL6Changed(QString)\0"
    "circleL7Changed(QString)\0"
    "circleL8Changed(QString)\0"
};

void CircleMaze::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CircleMaze *_t = static_cast<CircleMaze *>(_o);
        switch (_id) {
        case 0: _t->StartCircle_Exp((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int*(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 1: _t->StartCircle_T1((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 2: _t->StartCircle_T2((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 3: _t->ratChanged_E1((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->trialsChanged_E1((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->sZoneChanged_E1((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->ratChanged_TP1((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->trialsChanged_TP1((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->sZoneChanged_TP1((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->sITIChanged_TP1((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->ratChanged_TP2((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->trialsChanged_TP2((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->sZoneChanged_TP2((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->sITIChanged_TP2((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->delaySpinChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->experimentSTART_E1(); break;
        case 16: _t->experimentSTART_T1(); break;
        case 17: _t->experimentSTART_T2(); break;
        case 18: _t->requestExperimentEND(); break;
        case 19: _t->experimentEND(); break;
        case 20: _t->ardPortChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->ardBaudChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->testArd(); break;
        case 23: { bool _r = _t->isArdConnected();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 24: _t->addLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 25: _t->clearLog(); break;
        case 26: _t->tickTock(); break;
        case 27: _t->circleL1Changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 28: _t->circleL2Changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 29: _t->circleL3Changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 30: _t->circleL4Changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 31: _t->circleL5Changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 32: _t->circleL6Changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 33: _t->circleL7Changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 34: _t->circleL8Changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
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
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    }
    return _id;
}

// SIGNAL 0
void CircleMaze::StartCircle_Exp(int _t1, int _t2, int * _t3, int _t4, int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CircleMaze::StartCircle_T1(int _t1, int _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CircleMaze::StartCircle_T2(int _t1, int _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
