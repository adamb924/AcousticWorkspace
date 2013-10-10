/****************************************************************************
** Meta object code from reading C++ file 'plotviewtreewidget.h'
**
** Created: Tue Mar 29 14:43:36 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../plotviewtreewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plotviewtreewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PlotViewTreeWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      47,   20,   19,   19, 0x05,
      84,   74,   19,   19, 0x05,
     114,  100,   19,   19, 0x05,
     152,  135,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     188,  176,   19,   19, 0x08,
     222,   19,   19,   19, 0x08,
     231,   19,   19,   19, 0x08,
     247,  242,   19,   19, 0x08,
     263,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PlotViewTreeWidget[] = {
    "PlotViewTreeWidget\0\0plotIndex,hasSecondaryAxis\0"
    "setSecondaryAxis(int,bool)\0plotIndex\0"
    "removePlot(int)\0plot,waveform\0"
    "addWaveform(int,int)\0plot,spectrogram\0"
    "addSpectrogram(int,int)\0item,column\0"
    "doubleClick(QTreeWidgetItem*,int)\0"
    "remove()\0settings()\0does\0secondary(bool)\0"
    "moveToOtherAxis()\0"
};

const QMetaObject PlotViewTreeWidget::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_PlotViewTreeWidget,
      qt_meta_data_PlotViewTreeWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PlotViewTreeWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PlotViewTreeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PlotViewTreeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PlotViewTreeWidget))
        return static_cast<void*>(const_cast< PlotViewTreeWidget*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int PlotViewTreeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        case 4: doubleClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: remove(); break;
        case 6: settings(); break;
        case 7: secondary((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: moveToOtherAxis(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void PlotViewTreeWidget::setSecondaryAxis(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PlotViewTreeWidget::removePlot(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PlotViewTreeWidget::addWaveform(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void PlotViewTreeWidget::addSpectrogram(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
