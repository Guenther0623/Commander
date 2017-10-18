/****************************************************************************
** Meta object code from reading C++ file 'window.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      59,   57,   11,   11, 0x0a,
      80,   11,   11,   11, 0x0a,
      93,   11,   11,   11, 0x0a,
     104,   11,   11,   11, 0x08,
     117,   11,   11,   11, 0x08,
     130,   11,   11,   11, 0x08,
     150,   11,   11,   11, 0x08,
     170,   11,   11,   11, 0x08,
     186,   11,   11,   11, 0x08,
     196,   11,   11,   11, 0x08,
     226,   11,   11,   11, 0x08,
     256,   11,   11,   11, 0x08,
     286,   11,   11,   11, 0x08,
     302,   11,   11,   11, 0x08,
     318,   11,   11,   11, 0x08,
     334,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0,,,,,\0"
    "startDNMS_exp(int,int,int,int,int,int)\0"
    ",\0logSlot(QString,int)\0DNMS_Start()\0"
    "DNMS_End()\0testLogger()\0refreshLog()\0"
    "ardPortChanged(int)\0ardBaudChanged(int)\0"
    "expChanged(int)\0testArd()\0"
    "DNMS_textChanged_rat(QString)\0"
    "DNMS_textChanged_rew(QString)\0"
    "DNMS_textChanged_ite(QString)\0"
    "DNMS_delMode1()\0DNMS_delMode2()\0"
    "DNMS_trials56()\0DNMS_trials64()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->startDNMS_exp((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 1: _t->logSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->DNMS_Start(); break;
        case 3: _t->DNMS_End(); break;
        case 4: _t->testLogger(); break;
        case 5: _t->refreshLog(); break;
        case 6: _t->ardPortChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->ardBaudChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->expChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->testArd(); break;
        case 10: _t->DNMS_textChanged_rat((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->DNMS_textChanged_rew((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->DNMS_textChanged_ite((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->DNMS_delMode1(); break;
        case 14: _t->DNMS_delMode2(); break;
        case 15: _t->DNMS_trials56(); break;
        case 16: _t->DNMS_trials64(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::startDNMS_exp(int _t1, int _t2, int _t3, int _t4, int _t5, int _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
