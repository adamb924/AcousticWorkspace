/****************************************************************************
** Meta object code from reading C++ file 'curvesettingsdialog.h'
**
** Created: Tue Mar 29 14:43:54 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../curvesettingsdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'curvesettingsdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CurveSettingsDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x08,
      41,   20,   20,   20, 0x08,
      65,   59,   20,   20, 0x08,
      94,   89,   20,   20, 0x08,
     116,   20,   20,   20, 0x08,
     128,   59,   20,   20, 0x08,
     150,   89,   20,   20, 0x08,
     176,  170,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CurveSettingsDialog[] = {
    "CurveSettingsDialog\0\0SymbolBorderColor()\0"
    "SymbolFillColor()\0index\0SymbolStyleChanged(int)\0"
    "size\0SymbolSizeChange(int)\0LineColor()\0"
    "LineStyleChanged(int)\0LineSizeChange(int)\0"
    "value\0LineAntialiased(bool)\0"
};

const QMetaObject CurveSettingsDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CurveSettingsDialog,
      qt_meta_data_CurveSettingsDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CurveSettingsDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CurveSettingsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CurveSettingsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CurveSettingsDialog))
        return static_cast<void*>(const_cast< CurveSettingsDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int CurveSettingsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SymbolBorderColor(); break;
        case 1: SymbolFillColor(); break;
        case 2: SymbolStyleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: SymbolSizeChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: LineColor(); break;
        case 5: LineStyleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: LineSizeChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: LineAntialiased((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
