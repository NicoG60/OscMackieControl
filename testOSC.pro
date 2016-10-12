#-------------------------------------------------
#
# Project created by QtCreator 2016-10-05T13:50:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testOSC
TEMPLATE = app


SOURCES += main.cpp\
        Widget.cpp \
    RtMidi.cpp \
    MidiIO.cpp

HEADERS  += Widget.h \
    midiNote.h \
    RtMidi.h \
    MidiIO.h

FORMS    += Widget.ui

win32: LIBS += -L$$PWD/lib/ -losc
win32: LIBS += -lwinmm

INCLUDEPATH += $$PWD/osc/.
DEPENDPATH += $$PWD/osc/.

INCLUDEPATH += $$PWD/tools/.
DEPENDPATH += $$PWD/tools/.
