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
    DESTDIR = $$PWD/../build/debug
}
CONFIG(release, debug|release) {
    DESTDIR = $$PWD/../build/release
}

MOC_DIR = $$PWD/../tmp/moc
UI_DIR = $$PWD/../tmp/ui
RCC_DIR = $$PWD/../tmp/rcc
OBJECTS_DIR = $$PWD/../tmp/obj

LIBDIR = $$PWD/../thirdParty
OSCDIR = $$LIBDIR/osc-cpp-qt
MIDIDIR = $$LIBDIR/rtmidi

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

LIBS += -L$$OSCDIR/bin/release -losc
LIBS += -L$$MIDIDIR -lrtmidi

win32: { 
    RC_ICONS = $$PWD/../graphical/icon.ico

    QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$shell_path($$PWD/Overlays.ini)) $$quote($$shell_path($${DESTDIR}))
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += windeployqt $$quote($$shell_path($${DESTDIR}/$${TARGET}.exe)) -verbose=1
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$shell_path($${OSCDIR}/bin/release/osc.dll)) $$quote($$shell_path($${DESTDIR}))
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$shell_path($${MIDIDIR}/rtmidi.dll)) $$quote($$shell_path($${DESTDIR}))
}

osx: {
    ICON = $$PWD/../graphical/icon.icns

    QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$PWD/Overlays.ini) $$quote($${DESTDIR}/$${TARGET}.app/Contents/MacOS)
    # QMAKE_POST_LINK += &&
    # QMAKE_POST_LINK += macdeployqt $${DESTDIR}/$${TARGET}.app -verbose=1 -libpath=$${OSCDIR}/bin/release/ -libpath=$${MIDIDIR}/lib/osx/
}

INCLUDEPATH += $$OSCDIR
INCLUDEPATH += $$MIDIDIR
