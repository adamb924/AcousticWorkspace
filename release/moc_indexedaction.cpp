/****************************************************************************
** Meta object code from reading C++ file 'indexedaction.h'
**
** Created: Tue Mar 29 14:43:49 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../indexedaction.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'indexedaction.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_IndexedAction[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_IndexedAction[] = {
    "IndexedAction\0\0index\0indexClicked(int)\0"
    "mytrigger()\0"
};

const QMetaObject IndexedAction::staticMetaObject = {
    { &QAction::staticMetaObject, qt_meta_stringdata_IndexedAction,
      qt_meta_data_IndexedAction, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &IndexedAction::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *IndexedAction::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *IndexedAction::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IndexedAction))
        return static_cast<void*>(const_cast< IndexedAction*>(this));
    return QAction::qt_metacast(_clname);
}

int IndexedAction::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAction::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: indexClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: mytrigger(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void IndexedAction::indexClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
