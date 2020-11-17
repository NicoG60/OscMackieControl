#-------------------------------------------------
#
# Project created by QtCreator 2016-10-05T13:50:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OscMackieControl
TEMPLATE = app

MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp
OBJECTS_DIR = tmp

SOURCES += main.cpp \
    backend.cpp
HEADERS += \
    backend.h

LIBS += -L$$OUT_PWD/../third_party/QOsc/src/lib -lQOsc
LIBS += -L$$OUT_PWD/../third_party/QMidi/src/lib -lQMidi

INCLUDEPATH += $$PWD/../third_party/QOsc/include
INCLUDEPATH += $$PWD/../third_party/QMidi/include

win32: {
    RC_ICONS = $$PWD/../graphical/icon.ico
}

osx: {
    ICON = $$PWD/../graphical/icon.icns
}




