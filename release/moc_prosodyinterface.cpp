/****************************************************************************
** Meta object code from reading C++ file 'prosodyinterface.h'
**
** Created: Mon Mar 28 11:37:01 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../prosodyinterface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'prosodyinterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ProsodyInterface[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       1,   24, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   18,   17,   17, 0x0a,
      67,   62,   17,   17, 0x0a,

 // enums: name, flags, count, data
      93, 0x0,    2,   28,

 // enum data: key, value
      98, uint(ProsodyInterface::Spectrogram),
     110, uint(ProsodyInterface::Waveform),

       0        // eod
};

static const char qt_meta_stringdata_ProsodyInterface[] = {
    "ProsodyInterface\0\0left,right\0"
    "setHorizontalAxis(double,double)\0does\0"
    "setHasSecondaryAxis(bool)\0Type\0"
    "Spectrogram\0Waveform\0"
};

const QMetaObject ProsodyInterface::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ProsodyInterface,
      qt_meta_data_ProsodyInterface, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ProsodyInterface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ProsodyInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ProsodyInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ProsodyInterface))
        return static_cast<void*>(const_cast< ProsodyInterface*>(this));
    return QWidget::qt_metacast(_clname);
}

int ProsodyInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setHorizontalAxis((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: setHasSecondaryAxis((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
