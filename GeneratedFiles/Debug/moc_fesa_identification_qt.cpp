/****************************************************************************
** Meta object code from reading C++ file 'fesa_identification_qt.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fesa_identification_qt.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fesa_identification_qt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Fesa_Identification_Qt[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x0a,
      46,   23,   23,   23, 0x0a,
      62,   23,   23,   23, 0x0a,
      75,   23,   23,   23, 0x0a,
      95,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Fesa_Identification_Qt[] = {
    "Fesa_Identification_Qt\0\0identification_fase()\0"
    "close_program()\0add_person()\0"
    "return_pixmap_img()\0set_name_persona()\0"
};

void Fesa_Identification_Qt::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Fesa_Identification_Qt *_t = static_cast<Fesa_Identification_Qt *>(_o);
        switch (_id) {
        case 0: _t->identification_fase(); break;
        case 1: _t->close_program(); break;
        case 2: _t->add_person(); break;
        case 3: _t->return_pixmap_img(); break;
        case 4: _t->set_name_persona(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Fesa_Identification_Qt::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Fesa_Identification_Qt::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Fesa_Identification_Qt,
      qt_meta_data_Fesa_Identification_Qt, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Fesa_Identification_Qt::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Fesa_Identification_Qt::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Fesa_Identification_Qt::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Fesa_Identification_Qt))
        return static_cast<void*>(const_cast< Fesa_Identification_Qt*>(this));
    return QWidget::qt_metacast(_clname);
}

int Fesa_Identification_Qt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
