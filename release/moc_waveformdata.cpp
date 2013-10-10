/****************************************************************************
** Meta object code from reading C++ file 'waveformdata.h'
**
** Created: Tue Mar 29 14:43:52 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../waveformdata.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'waveformdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WaveformData[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   21,   14,   13, 0x0a,
      33,   21,   14,   13, 0x0a,
      50,   13,   43,   13, 0x0a,
      65,   13,   57,   13, 0x0a,
      73,   13,   57,   13, 0x0a,
      83,   81,   13,   13, 0x0a,
     103,  101,   13,   13, 0x0a,
     129,  121,   13,   13, 0x0a,
     152,   13,   43,   13, 0x0a,
     166,   13,   14,   13, 0x0a,
     173,   13,   14,   13, 0x0a,
     180,   13,   14,   13, 0x0a,
     200,  194,  189,   13, 0x0a,
     240,   13,   14,   13, 0x0a,
     263,   13,   14,   13, 0x0a,
     285,   21,   14,   13, 0x0a,
     307,   21,   14,   13, 0x0a,
     337,  332,   43,   13, 0x0a,
     371,   13,  363,   13, 0x0a,
     378,   13,  363,   13, 0x0a,
     391,  389,   13,   13, 0x0a,
     422,   13,  408,   13, 0x0a,
     437,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WaveformData[] = {
    "WaveformData\0\0double\0i\0x(size_t)\0"
    "y(size_t)\0size_t\0size()\0double*\0xData()\0"
    "yData()\0x\0setXData(double*)\0y\0"
    "setYData(double*)\0index,x\0"
    "setXDataAt(int,double)\0getNSamples()\0"
    "tMin()\0tMax()\0length()\0bool\0other\0"
    "checkCongruentWith(const WaveformData*)\0"
    "getSamplingFrequency()\0getNyquistFrequency()\0"
    "getYAtSample(quint32)\0getTimeAtSample(quint32)\0"
    "time\0getSampleFromTime(double)\0QString\0"
    "name()\0safeName()\0n\0setName(QString)\0"
    "QwtDoubleRect\0boundingRect()\0"
    "calculateMinMax()\0"
};

const QMetaObject WaveformData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WaveformData,
      qt_meta_data_WaveformData, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WaveformData::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WaveformData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WaveformData::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WaveformData))
        return static_cast<void*>(const_cast< WaveformData*>(this));
    if (!strcmp(_clname, "QwtData"))
        return static_cast< QwtData*>(const_cast< WaveformData*>(this));
    return QObject::qt_metacast(_clname);
}

int WaveformData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { double _r = x((*reinterpret_cast< size_t(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 1: { double _r = y((*reinterpret_cast< size_t(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 2: { size_t _r = size();
            if (_a[0]) *reinterpret_cast< size_t*>(_a[0]) = _r; }  break;
        case 3: { double* _r = xData();
            if (_a[0]) *reinterpret_cast< double**>(_a[0]) = _r; }  break;
        case 4: { double* _r = yData();
            if (_a[0]) *reinterpret_cast< double**>(_a[0]) = _r; }  break;
        case 5: setXData((*reinterpret_cast< double*(*)>(_a[1]))); break;
        case 6: setYData((*reinterpret_cast< double*(*)>(_a[1]))); break;
        case 7: setXDataAt((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 8: { size_t _r = getNSamples();
            if (_a[0]) *reinterpret_cast< size_t*>(_a[0]) = _r; }  break;
        case 9: { double _r = tMin();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 10: { double _r = tMax();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 11: { double _r = length();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 12: { bool _r = checkCongruentWith((*reinterpret_cast< const WaveformData*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: { double _r = getSamplingFrequency();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 14: { double _r = getNyquistFrequency();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 15: { double _r = getYAtSample((*reinterpret_cast< quint32(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 16: { double _r = getTimeAtSample((*reinterpret_cast< quint32(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 17: { size_t _r = getSampleFromTime((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< size_t*>(_a[0]) = _r; }  break;
        case 18: { QString _r = name();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 19: { QString _r = safeName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 20: setName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: { QwtDoubleRect _r = boundingRect();
            if (_a[0]) *reinterpret_cast< QwtDoubleRect*>(_a[0]) = _r; }  break;
        case 22: calculateMinMax(); break;
        default: ;
        }
        _id -= 23;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
