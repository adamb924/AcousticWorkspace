/****************************************************************************
** Meta object code from reading C++ file 'prosodyviewtreewidget.h'
**
** Created: Tue Mar 29 13:02:28 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../prosodyviewtreewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'prosodyviewtreewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ProsodyViewTreeWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      38,   23,   22,   22, 0x05,
      75,   65,   22,   22, 0x05,
     105,   91,   22,   22, 0x05,
     143,  126,   22,   22, 0x05,
     178,  167,   22,   22, 0x05,
     216,  200,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
     255,  243,   22,   22, 0x08,
     289,   22,   22,   22, 0x08,
     298,   22,   22,   22, 0x08,
     314,  309,   22,   22, 0x08,
     330,   22,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ProsodyViewTreeWidget[] = {
    "ProsodyViewTreeWidget\0\0plotIndex,does\0"
    "setSecondaryAxis(int,bool)\0plotIndex\0"
    "removePlot(int)\0plot,waveform\0"
    "addWaveform(int,int)\0plot,spectrogram\0"
    "addSpectrogram(int,int)\0plot,index\0"
    "removeSource(int,int)\0plotIndex,index\0"
    "editCurveSettings(int,int)\0item,column\0"
    "doubleClick(QTreeWidgetItem*,int)\0"
    "remove()\0settings()\0does\0secondary(bool)\0"
    "moveToOtherAxis()\0"
};

const QMetaObject ProsodyViewTreeWidget::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_ProsodyViewTreeWidget,
      qt_meta_data_ProsodyViewTreeWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ProsodyViewTreeWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ProsodyViewTreeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ProsodyViewTreeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ProsodyViewTreeWidget))
        return static_cast<void*>(const_cast< ProsodyViewTreeWidget*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int ProsodyViewTreeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setSecondaryAxis((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: removePlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: addWaveform((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: addSpectrogram((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: removeSource((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: editCurveSettings((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: doubleClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: remove(); break;
        case 8: settings(); break;
        case 9: secondary((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: moveToOtherAxis(); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void ProsodyViewTreeWidget::setSecondaryAxis(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ProsodyViewTreeWidget::removePlot(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ProsodyViewTreeWidget::addWaveform(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ProsodyViewTreeWidget::addSpectrogram(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ProsodyViewTreeWidget::removeSource(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ProsodyViewTreeWidget::editCurveSettings(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
