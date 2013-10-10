/****************************************************************************
** Meta object code from reading C++ file 'pluginviewtreewidget.h'
**
** Created: Tue Mar 29 14:43:41 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../pluginviewtreewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pluginviewtreewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PluginViewTreeWidget[] = {

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
      48,   22,   21,   21, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_PluginViewTreeWidget[] = {
    "PluginViewTreeWidget\0\0from,toPlugin,toSubplugin\0"
    "dropped(int,int,int)\0"
};

const QMetaObject PluginViewTreeWidget::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_PluginViewTreeWidget,
      qt_meta_data_PluginViewTreeWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PluginViewTreeWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PluginViewTreeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PluginViewTreeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PluginViewTreeWidget))
        return static_cast<void*>(const_cast< PluginViewTreeWidget*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int PluginViewTreeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: dropped((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void PluginViewTreeWidget::dropped(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
