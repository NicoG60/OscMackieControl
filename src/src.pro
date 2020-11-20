#-------------------------------------------------
#
# Project created by QtCreator 2016-10-05T13:50:52
#
#-------------------------------------------------

QT       += core gui network quick quickcontrols2

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OscMackieControl
TEMPLATE = app

VERSION = 2.0.0
DEFINES += GIT_VERSION=$$system(git rev-parse --short HEAD)
DEFINES += SOFT_VERSION=$$VERSION

MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp
OBJECTS_DIR = tmp

SOURCES += main.cpp \
    backend.cpp \
    iconmanager.cpp \
    mapping.cpp \
    oscmackiecontrolapp.cpp
HEADERS += \
    backend.h \
    iconmanager.h \
    mapping.h \
    oscmackiecontrolapp.h

LIBS += -L$$OUT_PWD/../third_party/QOsc/src/lib -lQOsc
LIBS += -L$$OUT_PWD/../third_party/QMidi/src/lib -lQMidi

INCLUDEPATH += $$PWD/../third_party/QOsc/include
INCLUDEPATH += $$PWD/../third_party/QMidi/include

include($$PWD/../third_party/QtAwesome/QtAwesome/QtAwesome.pri)

win32: {
    RC_ICONS = $$PWD/../graphical/icon.ico
}

osx: {
    ICON = $$PWD/../graphical/icon.icns
}

RESOURCES += \
    ../gui/qml.qrc




