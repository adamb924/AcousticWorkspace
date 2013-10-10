/****************************************************************************
** Meta object code from reading C++ file 'soundwidget.h'
**
** Created: Tue Mar 29 14:43:40 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../soundwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'soundwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SoundWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   13,   12,   12, 0x05,
      62,   12,   12,   12, 0x05,
      89,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     118,  109,   12,   12, 0x0a,
     149,  140,   12,   12, 0x0a,
     179,   12,   12,   12, 0x0a,
     187,  140,   12,   12, 0x0a,
     206,   12,   12,   12, 0x0a,
     213,  109,   12,   12, 0x0a,
     247,  241,   12,   12, 0x0a,
     267,  241,   12,   12, 0x0a,
     290,   12,   12,   12, 0x08,
     304,   12,   12,   12, 0x08,
     324,   12,   12,   12, 0x08,
     340,   12,   12,   12, 0x08,
     354,   12,   12,   12, 0x08,
     374,   12,   12,   12, 0x08,
     383,   12,   12,   12, 0x08,
     411,  400,   12,   12, 0x08,
     461,  450,   12,   12, 0x08,
     498,  493,   12,   12, 0x08,
     531,  493,   12,   12, 0x08,
     565,  558,   12,   12, 0x08,
     596,  558,   12,   12, 0x08,
     627,   12,   12,   12, 0x08,
     644,  639,   12,   12, 0x08,
     665,   12,   12,   12, 0x08,
     690,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SoundWidget[] = {
    "SoundWidget\0\0left,right\0"
    "horizontalScaleUpdated(double,double)\0"
    "requestScriptDataRefresh()\0"
    "scriptDataChanged()\0filename\0"
    "readFromFile(QString)\0fileName\0"
    "readTextGridFromFile(QString)\0close()\0"
    "loadSound(QString)\0save()\0"
    "writeProjectToFile(QString)\0index\0"
    "removeWaveform(int)\0removeSpectrogram(int)\0"
    "importSound()\0launchDataManager()\0"
    "newRegression()\0openProject()\0"
    "launchPlotManager()\0saveAs()\0"
    "importTextGrid()\0annotation\0"
    "addAnnotationMenu(IntervalAnnotation*)\0"
    "regression\0addRegression(RegressionModel*)\0"
    "data\0addSpectrogram(SpectrogramData*)\0"
    "addWaveform(WaveformData*)\0action\0"
    "regressionMenuAction(QAction*)\0"
    "annotationMenuAction(QAction*)\0"
    "runScript()\0code\0executeCode(QString)\0"
    "setupScriptEnvironment()\0setupScripting()\0"
};

const QMetaObject SoundWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SoundWidget,
      qt_meta_data_SoundWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SoundWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SoundWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SoundWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SoundWidget))
        return static_cast<void*>(const_cast< SoundWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int SoundWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: horizontalScaleUpdated((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: requestScriptDataRefresh(); break;
        case 2: scriptDataChanged(); break;
        case 3: readFromFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: readTextGridFromFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: close(); break;
        case 6: loadSound((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: save(); break;
        case 8: writeProjectToFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: removeWaveform((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: removeSpectrogram((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: importSound(); break;
        case 12: launchDataManager(); break;
        case 13: newRegression(); break;
        case 14: openProject(); break;
        case 15: launchPlotManager(); break;
        case 16: saveAs(); break;
        case 17: importTextGrid(); break;
        case 18: addAnnotationMenu((*reinterpret_cast< IntervalAnnotation*(*)>(_a[1]))); break;
        case 19: addRegression((*reinterpret_cast< RegressionModel*(*)>(_a[1]))); break;
        case 20: addSpectrogram((*reinterpret_cast< SpectrogramData*(*)>(_a[1]))); break;
        case 21: addWaveform((*reinterpret_cast< WaveformData*(*)>(_a[1]))); break;
        case 22: regressionMenuAction((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 23: annotationMenuAction((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 24: runScript(); break;
        case 25: executeCode((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 26: setupScriptEnvironment(); break;
        case 27: setupScripting(); break;
        default: ;
        }
        _id -= 28;
    }
    return _id;
}

// SIGNAL 0
void SoundWidget::horizontalScaleUpdated(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SoundWidget::requestScriptDataRefresh()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void SoundWidget::scriptDataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
