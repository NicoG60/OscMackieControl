TEMPLATE = subdirs

SUBDIRS += \
    src \
    qosc \
    qmidi

qosc.subdir = $$PWD/third_party/QOsc/src
qmidi.subdir = $$PWD/third_party/QMidi/src

src.depends = qosc qmidi
