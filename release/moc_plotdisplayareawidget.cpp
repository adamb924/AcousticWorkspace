/****************************************************************************
** Meta object code from reading C++ file 'plotdisplayareawidget.h'
**
** Created: Thu Dec 29 14:07:17 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../plotdisplayareawidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plotdisplayareawidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PlotDisplayAreaWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   23,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
      73,   65,   22,   22, 0x0a,
     113,  110,   22,   22, 0x2a,
     153,  142,   22,   22, 0x0a,
     191,   22,   22,   22, 0x0a,
     212,   22,   22,   22, 0x08,
     218,   22,   22,   22, 0x08,
     223,   22,   22,   22, 0x08,
     229,   22,   22,   22, 0x08,
     241,  235,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PlotDisplayAreaWidget[] = {
    "PlotDisplayAreaWidget\0\0left,right\0"
    "timeAxisChanged(double,double)\0pr,name\0"
    "addPlotView(PlotViewWidget*,QString)\0"
    "pr\0addPlotView(PlotViewWidget*)\0"
    "annotation\0addAnnotation(IntervalDisplayWidget*)\0"
    "drawVerticalLayout()\0all()\0in()\0out()\0"
    "sel()\0value\0updateTimeScaleFromSlider(double)\0"
};

const QMetaObject PlotDisplayAreaWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PlotDisplayAreaWidget,
      qt_meta_data_PlotDisplayAreaWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PlotDisplayAreaWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PlotDisplayAreaWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PlotDisplayAreaWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PlotDisplayAreaWidget))
        return static_cast<void*>(const_cast< PlotDisplayAreaWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int PlotDisplayAreaWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: timeAxisChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: addPlotView((*reinterpret_cast< PlotViewWidget*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: addPlotView((*reinterpret_cast< PlotViewWidget*(*)>(_a[1]))); break;
        case 3: addAnnotation((*reinterpret_cast< IntervalDisplayWidget*(*)>(_a[1]))); break;
        case 4: drawVerticalLayout(); break;
        case 5: all(); break;
        case 6: in(); break;
        case 7: out(); break;
        case 8: sel(); break;
        case 9: updateTimeScaleFromSlider((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void PlotDisplayAreaWidget::timeAxisChanged(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
