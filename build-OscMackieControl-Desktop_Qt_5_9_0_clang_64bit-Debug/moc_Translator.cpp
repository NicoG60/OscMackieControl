/****************************************************************************
** Meta object code from reading C++ file 'Translator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/Translator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Translator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Translator_t {
    QByteArrayData data[23];
    char stringdata0[229];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Translator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Translator_t qt_meta_stringdata_Translator = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Translator"
QT_MOC_LITERAL(1, 11, 11), // "midiNoteOff"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 4), // "chan"
QT_MOC_LITERAL(4, 29, 4), // "note"
QT_MOC_LITERAL(5, 34, 3), // "vel"
QT_MOC_LITERAL(6, 38, 10), // "midiNoteOn"
QT_MOC_LITERAL(7, 49, 15), // "midiKeyPressure"
QT_MOC_LITERAL(8, 65, 13), // "midiControler"
QT_MOC_LITERAL(9, 79, 7), // "control"
QT_MOC_LITERAL(10, 87, 5), // "value"
QT_MOC_LITERAL(11, 93, 11), // "midiProgram"
QT_MOC_LITERAL(12, 105, 7), // "program"
QT_MOC_LITERAL(13, 113, 19), // "midiChannelPressure"
QT_MOC_LITERAL(14, 133, 13), // "midiPitchBend"
QT_MOC_LITERAL(15, 147, 9), // "midiSysex"
QT_MOC_LITERAL(16, 157, 4), // "data"
QT_MOC_LITERAL(17, 162, 16), // "midiSystemCommon"
QT_MOC_LITERAL(18, 179, 6), // "status"
QT_MOC_LITERAL(19, 186, 18), // "midiSystemRealtime"
QT_MOC_LITERAL(20, 205, 7), // "readOSC"
QT_MOC_LITERAL(21, 213, 11), // "OscMessage*"
QT_MOC_LITERAL(22, 225, 3) // "msg"

    },
    "Translator\0midiNoteOff\0\0chan\0note\0vel\0"
    "midiNoteOn\0midiKeyPressure\0midiControler\0"
    "control\0value\0midiProgram\0program\0"
    "midiChannelPressure\0midiPitchBend\0"
    "midiSysex\0data\0midiSystemCommon\0status\0"
    "midiSystemRealtime\0readOSC\0OscMessage*\0"
    "msg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Translator[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   69,    2, 0x08 /* Private */,
       6,    3,   76,    2, 0x08 /* Private */,
       7,    3,   83,    2, 0x08 /* Private */,
       8,    3,   90,    2, 0x08 /* Private */,
      11,    2,   97,    2, 0x08 /* Private */,
      13,    2,  102,    2, 0x08 /* Private */,
      14,    2,  107,    2, 0x08 /* Private */,
      15,    1,  112,    2, 0x08 /* Private */,
      17,    1,  115,    2, 0x08 /* Private */,
      19,    1,  118,    2, 0x08 /* Private */,
      20,    1,  121,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,   10,
    QMetaType::Void, QMetaType::QByteArray,   16,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, 0x80000000 | 21,   22,

       0        // eod
};

void Translator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Translator *_t = static_cast<Translator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->midiNoteOff((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 1: _t->midiNoteOn((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 2: _t->midiKeyPressure((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 3: _t->midiControler((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 4: _t->midiProgram((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 5: _t->midiChannelPressure((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 6: _t->midiPitchBend((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 7: _t->midiSysex((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 8: _t->midiSystemCommon((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 9: _t->midiSystemRealtime((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 10: _t->readOSC((*reinterpret_cast< OscMessage*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Translator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Translator.data,
      qt_meta_data_Translator,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Translator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Translator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Translator.stringdata0))
        return static_cast<void*>(const_cast< Translator*>(this));
    return QObject::qt_metacast(_clname);
}

int Translator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
