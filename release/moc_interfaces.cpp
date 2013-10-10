/****************************************************************************
** Meta object code from reading C++ file 'interfaces.h'
**
** Created: Tue Mar 29 14:43:31 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../interfaces.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'interfaces.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AbstractMeasurement[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_AbstractMeasurement[] = {
    "AbstractMeasurement\0"
};

const QMetaObject AbstractMeasurement::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AbstractMeasurement,
      qt_meta_data_AbstractMeasurement, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AbstractMeasurement::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AbstractMeasurement::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AbstractMeasurement::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AbstractMeasurement))
        return static_cast<void*>(const_cast< AbstractMeasurement*>(this));
    return QObject::qt_metacast(_clname);
}

int AbstractMeasurement::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_AbstractWaveform2WaveformMeasure[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      39,   34,   33,   33, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_AbstractWaveform2WaveformMeasure[] = {
    "AbstractWaveform2WaveformMeasure\0\0"
    "data\0waveformCreated(WaveformData*)\0"
};

const QMetaObject AbstractWaveform2WaveformMeasure::staticMetaObject = {
    { &AbstractMeasurement::staticMetaObject, qt_meta_stringdata_AbstractWaveform2WaveformMeasure,
      qt_meta_data_AbstractWaveform2WaveformMeasure, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AbstractWaveform2WaveformMeasure::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AbstractWaveform2WaveformMeasure::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AbstractWaveform2WaveformMeasure::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AbstractWaveform2WaveformMeasure))
        return static_cast<void*>(const_cast< AbstractWaveform2WaveformMeasure*>(this));
    return AbstractMeasurement::qt_metacast(_clname);
}

int AbstractWaveform2WaveformMeasure::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractMeasurement::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: waveformCreated((*reinterpret_cast< WaveformData*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void AbstractWaveform2WaveformMeasure::waveformCreated(WaveformData * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_AbstractWaveform2SpectrogramMeasure[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      42,   37,   36,   36, 0x05,

 // slots: signature, parameters, type, tag, flags
      87,   36,   79,   36, 0x0a,
      94,   36,   79,   36, 0x0a,
     119,   36,  107,   36, 0x0a,
     129,  127,   36,   36, 0x0a,
     155,  143,   36,   36, 0x0a,
     193,  186,   36,   36, 0x0a,
     232,  222,   36,   36, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AbstractWaveform2SpectrogramMeasure[] = {
    "AbstractWaveform2SpectrogramMeasure\0"
    "\0data\0spectrogramCreated(SpectrogramData*)\0"
    "QString\0name()\0scriptName()\0QStringList\0"
    "names()\0i\0settings(int)\0label,value\0"
    "setParameter(QString,QVariant)\0i,data\0"
    "calculate(int,WaveformData*)\0name,data\0"
    "calculate(QString,WaveformData*)\0"
};

const QMetaObject AbstractWaveform2SpectrogramMeasure::staticMetaObject = {
    { &AbstractMeasurement::staticMetaObject, qt_meta_stringdata_AbstractWaveform2SpectrogramMeasure,
      qt_meta_data_AbstractWaveform2SpectrogramMeasure, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AbstractWaveform2SpectrogramMeasure::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AbstractWaveform2SpectrogramMeasure::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AbstractWaveform2SpectrogramMeasure::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AbstractWaveform2SpectrogramMeasure))
        return static_cast<void*>(const_cast< AbstractWaveform2SpectrogramMeasure*>(this));
    return AbstractMeasurement::qt_metacast(_clname);
}

int AbstractWaveform2SpectrogramMeasure::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractMeasurement::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: spectrogramCreated((*reinterpret_cast< SpectrogramData*(*)>(_a[1]))); break;
        case 1: { QString _r = name();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 2: { QString _r = scriptName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: { QStringList _r = names();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        case 4: settings((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: setParameter((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 6: calculate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< WaveformData*(*)>(_a[2]))); break;
        case 7: calculate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< WaveformData*(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void AbstractWaveform2SpectrogramMeasure::spectrogramCreated(SpectrogramData * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_AbstractSpectrogram2WaveformMeasure[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      42,   37,   36,   36, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_AbstractSpectrogram2WaveformMeasure[] = {
    "AbstractSpectrogram2WaveformMeasure\0"
    "\0data\0waveformCreated(WaveformData*)\0"
};

const QMetaObject AbstractSpectrogram2WaveformMeasure::staticMetaObject = {
    { &AbstractMeasurement::staticMetaObject, qt_meta_stringdata_AbstractSpectrogram2WaveformMeasure,
      qt_meta_data_AbstractSpectrogram2WaveformMeasure, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AbstractSpectrogram2WaveformMeasure::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AbstractSpectrogram2WaveformMeasure::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AbstractSpectrogram2WaveformMeasure::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AbstractSpectrogram2WaveformMeasure))
        return static_cast<void*>(const_cast< AbstractSpectrogram2WaveformMeasure*>(this));
    return AbstractMeasurement::qt_metacast(_clname);
}

int AbstractSpectrogram2WaveformMeasure::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractMeasurement::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: waveformCreated((*reinterpret_cast< WaveformData*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void AbstractSpectrogram2WaveformMeasure::waveformCreated(WaveformData * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_AbstractSpectrogram2SpectrogramMeasure[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      45,   40,   39,   39, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_AbstractSpectrogram2SpectrogramMeasure[] = {
    "AbstractSpectrogram2SpectrogramMeasure\0"
    "\0data\0spectrogramCreated(SpectrogramData*)\0"
};

const QMetaObject AbstractSpectrogram2SpectrogramMeasure::staticMetaObject = {
    { &AbstractMeasurement::staticMetaObject, qt_meta_stringdata_AbstractSpectrogram2SpectrogramMeasure,
      qt_meta_data_AbstractSpectrogram2SpectrogramMeasure, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AbstractSpectrogram2SpectrogramMeasure::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AbstractSpectrogram2SpectrogramMeasure::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AbstractSpectrogram2SpectrogramMeasure::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AbstractSpectrogram2SpectrogramMeasure))
        return static_cast<void*>(const_cast< AbstractSpectrogram2SpectrogramMeasure*>(this));
    return AbstractMeasurement::qt_metacast(_clname);
}

int AbstractSpectrogram2SpectrogramMeasure::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractMeasurement::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: spectrogramCreated((*reinterpret_cast< SpectrogramData*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void AbstractSpectrogram2SpectrogramMeasure::spectrogramCreated(SpectrogramData * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
