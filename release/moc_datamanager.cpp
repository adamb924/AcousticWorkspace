/****************************************************************************
** Meta object code from reading C++ file 'datamanager.h'
**
** Created: Tue Mar 29 10:59:09 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../datamanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datamanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DataManagerDialog[] = {

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
      45,   19,   18,   18, 0x0a,
      66,   19,   18,   18, 0x0a,
      87,   19,   18,   18, 0x0a,
     108,   19,   18,   18, 0x0a,
     135,  129,   18,   18, 0x0a,
     155,  129,   18,   18, 0x0a,
     178,  129,   18,   18, 0x0a,
     198,  129,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DataManagerDialog[] = {
    "DataManagerDialog\0\0from,toPlugin,toSubplugin\0"
    "w2wDrop(int,int,int)\0w2sDrop(int,int,int)\0"
    "s2wDrop(int,int,int)\0s2sDrop(int,int,int)\0"
    "index\0removeWaveform(int)\0"
    "removeSpectrogram(int)\0renameWaveform(int)\0"
    "renameSpectrogram(int)\0"
};

const QMetaObject DataManagerDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DataManagerDialog,
      qt_meta_data_DataManagerDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DataManagerDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DataManagerDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DataManagerDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataManagerDialog))
        return static_cast<void*>(const_cast< DataManagerDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int DataManagerDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: w2wDrop((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: w2sDrop((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: s2wDrop((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: s2sDrop((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: removeWaveform((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: removeSpectrogram((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: renameWaveform((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: renameSpectrogram((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
