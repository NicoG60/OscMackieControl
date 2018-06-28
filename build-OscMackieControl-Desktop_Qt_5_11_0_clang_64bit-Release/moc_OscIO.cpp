/****************************************************************************
** Meta object code from reading C++ file 'OscIO.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/OscIO.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OscIO.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OscIO_t {
    QByteArrayData data[17];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OscIO_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OscIO_t qt_meta_stringdata_OscIO = {
    {
QT_MOC_LITERAL(0, 0, 5), // "OscIO"
QT_MOC_LITERAL(1, 6, 10), // "oscMessage"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 11), // "OscMessage*"
QT_MOC_LITERAL(4, 30, 3), // "msg"
QT_MOC_LITERAL(5, 34, 7), // "sendOsc"
QT_MOC_LITERAL(6, 42, 4), // "addr"
QT_MOC_LITERAL(7, 47, 1), // "b"
QT_MOC_LITERAL(8, 49, 1), // "c"
QT_MOC_LITERAL(9, 51, 1), // "f"
QT_MOC_LITERAL(10, 53, 1), // "i"
QT_MOC_LITERAL(11, 55, 4), // "port"
QT_MOC_LITERAL(12, 60, 6), // "status"
QT_MOC_LITERAL(13, 67, 5), // "data1"
QT_MOC_LITERAL(14, 73, 5), // "data2"
QT_MOC_LITERAL(15, 79, 1), // "s"
QT_MOC_LITERAL(16, 81, 7) // "readOSC"

    },
    "OscIO\0oscMessage\0\0OscMessage*\0msg\0"
    "sendOsc\0addr\0b\0c\0f\0i\0port\0status\0data1\0"
    "data2\0s\0readOSC"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OscIO[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   57,    2, 0x0a /* Public */,
       5,    2,   62,    2, 0x0a /* Public */,
       5,    2,   67,    2, 0x0a /* Public */,
       5,    2,   72,    2, 0x0a /* Public */,
       5,    5,   77,    2, 0x0a /* Public */,
       5,    2,   88,    2, 0x0a /* Public */,
      16,    0,   93,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Char,    6,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::Float,    6,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    6,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::UChar, QMetaType::UChar, QMetaType::UChar, QMetaType::UChar,    6,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,   15,
    QMetaType::Void,

       0        // eod
};

void OscIO::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OscIO *_t = static_cast<OscIO *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->oscMessage((*reinterpret_cast< OscMessage*(*)>(_a[1]))); break;
        case 1: _t->sendOsc((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->sendOsc((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 3: _t->sendOsc((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 4: _t->sendOsc((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->sendOsc((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2])),(*reinterpret_cast< uchar(*)>(_a[3])),(*reinterpret_cast< uchar(*)>(_a[4])),(*reinterpret_cast< uchar(*)>(_a[5]))); break;
        case 6: _t->sendOsc((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: _t->readOSC(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OscIO::*)(OscMessage * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OscIO::oscMessage)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OscIO::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_OscIO.data,
      qt_meta_data_OscIO,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *OscIO::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OscIO::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OscIO.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int OscIO::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void OscIO::oscMessage(OscMessage * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
