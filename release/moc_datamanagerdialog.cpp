/****************************************************************************
** Meta object code from reading C++ file 'datamanagerdialog.h'
**
** Created: Tue Mar 29 14:43:56 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../datamanagerdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datamanagerdialog.h' doesn't include <QObject>."
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
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   19,   18,   18, 0x05,
      45,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      68,   18,   18,   18, 0x08,
      86,   18,   18,   18, 0x08,
     104,   18,   18,   18, 0x08,
     122,   18,   18,   18, 0x08,
     140,   18,   18,   18, 0x08,
     163,   18,   18,   18, 0x08,
     215,  189,   18,   18, 0x0a,
     236,  189,   18,   18, 0x0a,
     257,  189,   18,   18, 0x0a,
     278,  189,   18,   18, 0x0a,
     299,   19,   18,   18, 0x0a,
     319,   19,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DataManagerDialog[] = {
    "DataManagerDialog\0\0index\0removeWaveform(int)\0"
    "removeSpectrogram(int)\0populateW2wTree()\0"
    "populateW2sTree()\0populateS2wTree()\0"
    "populateS2sTree()\0populateWaveformTree()\0"
    "populateSpectrogramTree()\0"
    "from,toPlugin,toSubplugin\0"
    "w2wDrop(int,int,int)\0w2sDrop(int,int,int)\0"
    "s2wDrop(int,int,int)\0s2sDrop(int,int,int)\0"
    "renameWaveform(int)\0renameSpectrogram(int)\0"
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
        case 0: removeWaveform((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: removeSpectrogram((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: populateW2wTree(); break;
        case 3: populateW2sTree(); break;
        case 4: populateS2wTree(); break;
        case 5: populateS2sTree(); break;
        case 6: populateWaveformTree(); break;
        case 7: populateSpectrogramTree(); break;
        case 8: w2wDrop((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 9: w2sDrop((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 10: s2wDrop((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 11: s2sDrop((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 12: renameWaveform((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: renameSpectrogram((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void DataManagerDialog::removeWaveform(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataManagerDialog::removeSpectrogram(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
