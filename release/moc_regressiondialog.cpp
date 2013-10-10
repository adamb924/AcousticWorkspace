/****************************************************************************
** Meta object code from reading C++ file 'regressiondialog.h'
**
** Created: Tue Mar 29 14:43:45 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../regressiondialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'regressiondialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RegressionDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      59,   17,   17,   17, 0x08,
      67,   17,   17,   17, 0x08,
      75,   17,   17,   17, 0x08,
      84,   17,   17,   17, 0x08,
     105,  100,   17,   17, 0x08,
     148,  100,   17,   17, 0x08,
     183,  100,   17,   17, 0x08,
     229,   17,   17,   17, 0x08,
     251,   17,   17,   17, 0x08,
     269,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RegressionDialog[] = {
    "RegressionDialog\0\0model\0"
    "regressionObject(RegressionModel*)\0"
    "int2x()\0int3x()\0intAll()\0int2xSpecific()\0"
    "item\0independentSimpleChanged(QListWidgetItem*)\0"
    "dependentChanged(QListWidgetItem*)\0"
    "dependentSpectrogramChanged(QListWidgetItem*)\0"
    "calculateRegression()\0createRDataCode()\0"
    "saveRegression()\0"
};

const QMetaObject RegressionDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_RegressionDialog,
      qt_meta_data_RegressionDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RegressionDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RegressionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RegressionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RegressionDialog))
        return static_cast<void*>(const_cast< RegressionDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int RegressionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: regressionObject((*reinterpret_cast< RegressionModel*(*)>(_a[1]))); break;
        case 1: int2x(); break;
        case 2: int3x(); break;
        case 3: intAll(); break;
        case 4: int2xSpecific(); break;
        case 5: independentSimpleChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 6: dependentChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 7: dependentSpectrogramChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 8: calculateRegression(); break;
        case 9: createRDataCode(); break;
        case 10: saveRegression(); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void RegressionDialog::regressionObject(RegressionModel * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
