/****************************************************************************
** Meta object code from reading C++ file 'MidiIO.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/MidiIO.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MidiIO.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MidiIO_t {
    QByteArrayData data[29];
    char stringdata0[324];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MidiIO_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MidiIO_t qt_meta_stringdata_MidiIO = {
    {
QT_MOC_LITERAL(0, 0, 6), // "MidiIO"
QT_MOC_LITERAL(1, 7, 11), // "midiNoteOff"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 4), // "chan"
QT_MOC_LITERAL(4, 25, 4), // "note"
QT_MOC_LITERAL(5, 30, 3), // "vel"
QT_MOC_LITERAL(6, 34, 10), // "midiNoteOn"
QT_MOC_LITERAL(7, 45, 15), // "midiKeyPressure"
QT_MOC_LITERAL(8, 61, 13), // "midiControler"
QT_MOC_LITERAL(9, 75, 7), // "control"
QT_MOC_LITERAL(10, 83, 5), // "value"
QT_MOC_LITERAL(11, 89, 11), // "midiProgram"
QT_MOC_LITERAL(12, 101, 7), // "program"
QT_MOC_LITERAL(13, 109, 19), // "midiChannelPressure"
QT_MOC_LITERAL(14, 129, 13), // "midiPitchBend"
QT_MOC_LITERAL(15, 143, 9), // "midiSysex"
QT_MOC_LITERAL(16, 153, 4), // "data"
QT_MOC_LITERAL(17, 158, 16), // "midiSystemCommon"
QT_MOC_LITERAL(18, 175, 6), // "status"
QT_MOC_LITERAL(19, 182, 18), // "midiSystemRealtime"
QT_MOC_LITERAL(20, 201, 11), // "sendNoteOff"
QT_MOC_LITERAL(21, 213, 10), // "sendNoteOn"
QT_MOC_LITERAL(22, 224, 15), // "sendKeyPressure"
QT_MOC_LITERAL(23, 240, 13), // "sendControler"
QT_MOC_LITERAL(24, 254, 11), // "sendProgram"
QT_MOC_LITERAL(25, 266, 19), // "sendChannelPressure"
QT_MOC_LITERAL(26, 286, 13), // "sendPitchBend"
QT_MOC_LITERAL(27, 300, 9), // "sendSysex"
QT_MOC_LITERAL(28, 310, 13) // "readMidiInput"

    },
    "MidiIO\0midiNoteOff\0\0chan\0note\0vel\0"
    "midiNoteOn\0midiKeyPressure\0midiControler\0"
    "control\0value\0midiProgram\0program\0"
    "midiChannelPressure\0midiPitchBend\0"
    "midiSysex\0data\0midiSystemCommon\0status\0"
    "midiSystemRealtime\0sendNoteOff\0"
    "sendNoteOn\0sendKeyPressure\0sendControler\0"
    "sendProgram\0sendChannelPressure\0"
    "sendPitchBend\0sendSysex\0readMidiInput"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MidiIO[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,  109,    2, 0x06 /* Public */,
       6,    3,  116,    2, 0x06 /* Public */,
       7,    3,  123,    2, 0x06 /* Public */,
       8,    3,  130,    2, 0x06 /* Public */,
      11,    2,  137,    2, 0x06 /* Public */,
      13,    2,  142,    2, 0x06 /* Public */,
      14,    2,  147,    2, 0x06 /* Public */,
      15,    1,  152,    2, 0x06 /* Public */,
      17,    1,  155,    2, 0x06 /* Public */,
      19,    1,  158,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      20,    3,  161,    2, 0x0a /* Public */,
      21,    3,  168,    2, 0x0a /* Public */,
      22,    3,  175,    2, 0x0a /* Public */,
      23,    3,  182,    2, 0x0a /* Public */,
      24,    2,  189,    2, 0x0a /* Public */,
      25,    2,  194,    2, 0x0a /* Public */,
      26,    2,  199,    2, 0x0a /* Public */,
      27,    1,  204,    2, 0x0a /* Public */,
      28,    0,  207,    2, 0x08 /* Private */,

 // signals: parameters
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

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,   10,
    QMetaType::Void, QMetaType::QByteArray,   16,
    QMetaType::Void,

       0        // eod
};

void MidiIO::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MidiIO *_t = static_cast<MidiIO *>(_o);
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
        case 10: _t->sendNoteOff((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 11: _t->sendNoteOn((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 12: _t->sendKeyPressure((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 13: _t->sendControler((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 14: _t->sendProgram((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 15: _t->sendChannelPressure((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 16: _t->sendPitchBend((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 17: _t->sendSysex((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 18: _t->readMidiInput(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MidiIO::*_t)(const int , const int , const int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MidiIO::midiNoteOff)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MidiIO::*_t)(const int , const int , const int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MidiIO::midiNoteOn)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MidiIO::*_t)(const int , const int , const int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MidiIO::midiKeyPressure)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MidiIO::*_t)(const int , const int , const int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MidiIO::midiControler)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MidiIO::*_t)(const int , const int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MidiIO::midiProgram)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (MidiIO::*_t)(const int , const int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MidiIO::midiChannelPressure)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (MidiIO::*_t)(const int , const int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MidiIO::midiPitchBend)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (MidiIO::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MidiIO::midiSysex)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (MidiIO::*_t)(const int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MidiIO::midiSystemCommon)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (MidiIO::*_t)(const int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MidiIO::midiSystemRealtime)) {
                *result = 9;
                return;
            }
        }
    }
}

const QMetaObject MidiIO::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MidiIO.data,
      qt_meta_data_MidiIO,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MidiIO::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MidiIO::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MidiIO.stringdata0))
        return static_cast<void*>(const_cast< MidiIO*>(this));
    return QObject::qt_metacast(_clname);
}

int MidiIO::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void MidiIO::midiNoteOff(const int _t1, const int _t2, const int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MidiIO::midiNoteOn(const int _t1, const int _t2, const int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MidiIO::midiKeyPressure(const int _t1, const int _t2, const int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MidiIO::midiControler(const int _t1, const int _t2, const int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MidiIO::midiProgram(const int _t1, const int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MidiIO::midiChannelPressure(const int _t1, const int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MidiIO::midiPitchBend(const int _t1, const int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MidiIO::midiSysex(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MidiIO::midiSystemCommon(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void MidiIO::midiSystemRealtime(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
