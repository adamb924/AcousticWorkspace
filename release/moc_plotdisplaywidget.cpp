/****************************************************************************
** Meta object code from reading C++ file 'plotdisplaywidget.h'
**
** Created: Tue Mar 29 11:28:49 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../plotdisplaywidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plotdisplaywidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PlotDisplayWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      76,   68,   18,   18, 0x0a,
     123,  112,   18,   18, 0x0a,
     161,   18,   18,   18, 0x0a,
     182,   18,   18,   18, 0x08,
     188,   18,   18,   18, 0x08,
     193,   18,   18,   18, 0x08,
     199,   18,   18,   18, 0x08,
     211,  205,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PlotDisplayWidget[] = {
    "PlotDisplayWidget\0\0left,right\0"
    "horizontalScaleUpdated(double,double)\0"
    "pr,name\0addProsody(PlotViewWidget*,QString)\0"
    "annotation\0addAnnotation(IntervalDisplayWidget*)\0"
    "drawVerticalLayout()\0all()\0in()\0out()\0"
    "sel()\0value\0sliderChanged(double)\0"
};

const QMetaObject PlotDisplayWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PlotDisplayWidget,
      qt_meta_data_PlotDisplayWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PlotDisplayWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PlotDisplayWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PlotDisplayWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PlotDisplayWidget))
        return static_cast<void*>(const_cast< PlotDisplayWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int PlotDisplayWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: horizontalScaleUpdated((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: addProsody((*reinterpret_cast< PlotViewWidget*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: addAnnotation((*reinterpret_cast< IntervalDisplayWidget*(*)>(_a[1]))); break;
        case 3: drawVerticalLayout(); break;
        case 4: all(); break;
        case 5: in(); break;
        case 6: out(); break;
        case 7: sel(); break;
        case 8: sliderChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void PlotDisplayWidget::horizontalScaleUpdated(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
