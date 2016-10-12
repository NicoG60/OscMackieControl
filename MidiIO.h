#ifndef MIDIIO_H
#define MIDIIO_H

#include <QObject>
#include <QTimer>
#include "RtMidi.h"

#include "midiNote.h"


class MidiIO : public QObject
{
    Q_OBJECT

public:
    MidiIO(QObject* parent = 0);
    ~MidiIO();

    bool isOpen() { return midiIn->isPortOpen() && midiOut->isPortOpen(); }

    QStringList getAvailableInputPort();
    QStringList getAvailableOutputPort();

    bool connectInputPort(uint index);
    bool connectInputPort(QString name);

    bool connectOutputPort(uint index);
    bool connectOutputPort(QString name);

    QString getStatus();

signals:
    void midiNoteOff(const int chan, const int note, const int vel);
    void midiNoteOn(const int chan, const int note, const int vel);
    void midiKeyPressure(const int chan, const int note, const int vel);
    void midiControler(const int chan, const int control, const int value);
    void midiProgram(const int chan, const int program);
    void midiChannelPressure(const int chan, const int value);
    void midiPitchBend(const int chan, const int value);
    void midiSysex(const QByteArray &data);
    void midiSystemCommon(const int status);
    void midiSystemRealtime(const int status);

public slots:
    void sendNoteOff(const int chan, const int note, const int vel);
    void sendNoteOn(const int chan, const int note, const int vel);
    void sendKeyPressure(const int chan, const int note, const int vel);
    void sendControler(const int chan, const int control, const int value);
    void sendProgram(const int chan, const int program);
    void sendChannelPressure(const int chan, const int value);
    void sendPitchBend(const int chan, const int value);
    void sendSysex(const QByteArray &data);

private slots:
    void readMidiInput();

private:
    RtMidiIn*   midiIn;
    RtMidiOut*  midiOut;

    int         inPort;
    int         outPort;

    QTimer*     timer;
};

#endif // MIDIIO_H
