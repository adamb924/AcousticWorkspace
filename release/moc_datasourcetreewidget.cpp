/****************************************************************************
** Meta object code from reading C++ file 'datasourcetreewidget.h'
**
** Created: Tue Mar 29 14:43:53 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../datasourcetreewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datasourcetreewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DataSourceTreeWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   22,   21,   21, 0x05,
      46,   22,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      68,   21,   21,   21, 0x08,
      77,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DataSourceTreeWidget[] = {
    "DataSourceTreeWidget\0\0i\0removeItemSignal(int)\0"
    "renameItemSignal(int)\0remove()\0rename()\0"
};

const QMetaObject DataSourceTreeWidget::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_DataSourceTreeWidget,
      qt_meta_data_DataSourceTreeWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DataSourceTreeWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DataSourceTreeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DataSourceTreeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataSourceTreeWidget))
        return static_cast<void*>(const_cast< DataSourceTreeWidget*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int DataSourceTreeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: removeItemSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: renameItemSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: remove(); break;
        case 3: rename(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void DataSourceTreeWidget::removeItemSignal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataSourceTreeWidget::renameItemSignal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
