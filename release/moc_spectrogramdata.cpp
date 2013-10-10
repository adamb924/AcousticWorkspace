/****************************************************************************
** Meta object code from reading C++ file 'spectrogramdata.h'
**
** Created: Tue Mar 29 14:43:51 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../spectrogramdata.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spectrogramdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SpectrogramData[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      31,   16,   17,   16, 0x0a,
      63,   16,   46,   16, 0x0a,
      88,   16,   70,   16, 0x0a,
     107,  103,   96,   16, 0x0a,
     133,  131,   96,   16, 0x0a,
     159,   16,  151,   16, 0x0a,
     167,   16,  151,   16, 0x0a,
     189,  187,  182,   16, 0x0a,
     211,  209,  182,   16, 0x0a,
     244,  187,  236,   16, 0x0a,
     266,  187,  236,   16, 0x0a,
     288,  187,  236,   16, 0x0a,
     314,  187,  236,   16, 0x0a,
     344,  340,   96,   16, 0x0a,
     365,  340,   96,   16, 0x0a,
     410,   16,  402,   16, 0x0a,
     417,   16,  402,   16, 0x0a,
     430,  428,   16,   16, 0x0a,
     447,   16,   96,   16, 0x0a,
     465,   16,   96,   16, 0x0a,
     479,   16,  236,   16, 0x0a,
     495,   16,  236,   16, 0x0a,
     515,  131,   96,   16, 0x0a,
     537,  131,   96,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SpectrogramData[] = {
    "SpectrogramData\0\0QwtDoubleRect\0"
    "boundingRect()\0SpectrogramData*\0copy()\0"
    "QwtDoubleInterval\0range()\0double\0t,f\0"
    "dataAt(quint32,quint32)\0i\0flatdata(quint32)\0"
    "double*\0pdata()\0pfrequencies()\0bool\0"
    "t\0inTimeRange(double)\0f\0"
    "inFrequencyRange(double)\0quint32\0"
    "timeStepAbove(double)\0timeStepBelow(double)\0"
    "frequencyBinAbove(double)\0"
    "frequencyBinBelow(double)\0x,y\0"
    "value(double,double)\0"
    "bilinearInterpolation(double,double)\0"
    "QString\0name()\0safeName()\0n\0"
    "setName(QString)\0getWindowLength()\0"
    "getTimeStep()\0getNTimeSteps()\0"
    "getNFrequencyBins()\0getTimeFromIndex(int)\0"
    "getFrequencyFromIndex(int)\0"
};

const QMetaObject SpectrogramData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SpectrogramData,
      qt_meta_data_SpectrogramData, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SpectrogramData::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SpectrogramData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SpectrogramData::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SpectrogramData))
        return static_cast<void*>(const_cast< SpectrogramData*>(this));
    if (!strcmp(_clname, "QwtRasterData"))
        return static_cast< QwtRasterData*>(const_cast< SpectrogramData*>(this));
    return QObject::qt_metacast(_clname);
}

int SpectrogramData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QwtDoubleRect _r = boundingRect();
            if (_a[0]) *reinterpret_cast< QwtDoubleRect*>(_a[0]) = _r; }  break;
        case 1: { SpectrogramData* _r = copy();
            if (_a[0]) *reinterpret_cast< SpectrogramData**>(_a[0]) = _r; }  break;
        case 2: { QwtDoubleInterval _r = range();
            if (_a[0]) *reinterpret_cast< QwtDoubleInterval*>(_a[0]) = _r; }  break;
        case 3: { double _r = dataAt((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 4: { double _r = flatdata((*reinterpret_cast< quint32(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 5: { double* _r = pdata();
            if (_a[0]) *reinterpret_cast< double**>(_a[0]) = _r; }  break;
        case 6: { double* _r = pfrequencies();
            if (_a[0]) *reinterpret_cast< double**>(_a[0]) = _r; }  break;
        case 7: { bool _r = inTimeRange((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { bool _r = inFrequencyRange((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: { quint32 _r = timeStepAbove((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = _r; }  break;
        case 10: { quint32 _r = timeStepBelow((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = _r; }  break;
        case 11: { quint32 _r = frequencyBinAbove((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = _r; }  break;
        case 12: { quint32 _r = frequencyBinBelow((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = _r; }  break;
        case 13: { double _r = value((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 14: { double _r = bilinearInterpolation((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 15: { QString _r = name();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 16: { QString _r = safeName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 17: setName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: { double _r = getWindowLength();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 19: { double _r = getTimeStep();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 20: { quint32 _r = getNTimeSteps();
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = _r; }  break;
        case 21: { quint32 _r = getNFrequencyBins();
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = _r; }  break;
        case 22: { double _r = getTimeFromIndex((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 23: { double _r = getFrequencyFromIndex((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 24;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
