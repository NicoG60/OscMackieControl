#-------------------------------------------------
#
# Project created by QtCreator 2016-10-05T13:50:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OscMackieControl
TEMPLATE = app

CONFIG(debug, debug|release) {
    DESTDIR = ../build/debug
}
CONFIG(release, debug|release) {
    DESTDIR = ../build/release
}

MOC_DIR = ../tmp/moc
UI_DIR = ../tmp/ui
RCC_DIR = ../tmp/rcc
OBJECTS_DIR = ../tmp/obj

LIBDIR = ../thirdParty
OSCDIR = $$LIBDIR/libosc
MIDIDIR = $$LIBDIR/librtmidi

SOURCES  += main.cpp\
            Widget.cpp \
            MidiIO.cpp \
            OscIO.cpp \
            Translator.cpp

HEADERS  += Widget.h \
            midiNote.h \
            MidiIO.h \
            OscIO.h \
            Translator.h

FORMS    += Widget.ui

win32: {
    LIBS += -L$$OSCDIR/lib/win32 -losc
    LIBS += -L$$MIDIDIR/lib/win32 -lrtmidi
    RC_ICONS = ../graphical/icon.ico

    QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$shell_path(Overlays.ini)) $$quote($$shell_path($${DESTDIR}))
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += windeployqt $$quote($$shell_path($${DESTDIR}/$${TARGET}.exe)) -verbose=1
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$shell_path($${OSCDIR}/lib/win32/osc.dll)) $$quote($$shell_path($${DESTDIR}))
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$shell_path($${MIDIDIR}/lib/win32/librtmidi.dll)) $$quote($$shell_path($${DESTDIR}))
}

osx: {
    LIBS += -L$$OSCDIR/lib/osx -losc
    LIBS += -L$$MIDIDIR/lib/osx -lrtmidi
    ICON = ../graphical/icon.icns

    QMAKE_POST_LINK += $$QMAKE_COPY $$quote(Overlays.ini) $$quote($${DESTDIR}/$${TARGET}.app/Contents/MacOS)
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += macdeployqt $${DESTDIR}/$${TARGET}.app -verbose=1 -libpath=$${OSCDIR}/lib/osx/ -libpath=$${MIDIDIR}/lib/osx/
}

INCLUDEPATH += $$OSCDIR/include
INCLUDEPATH += $$MIDIDIR/include
