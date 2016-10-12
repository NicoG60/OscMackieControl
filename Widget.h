#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QSettings>
#include <QTimer>

#include "osc/composer/OscMessageComposer.h"
#include "osc/reader/OscReader.h"
#include "osc/reader/OscMessage.h"
#include "MidiIO.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    enum Protocol {
        OSC,
        MIDI
    };

    explicit Widget(QWidget *parent = 0);
    ~Widget();



private slots:
    void on_btnConnectOSC_clicked();
    void on_btnConnectMidi_clicked();

private slots:
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

    void readOSC();

private:

    bool isReady();
    void refreshMIDIPort();
    void connectOSC();
    void connectMIDI();
    void refreshStatus();

    void bangNote(int chan, int note, int vel);
    void sendOsc(OscMessageComposer *msg);
    void sendOsc(QString addr, float value);
    void sendOsc(QString addr, QString value);
    void btnAction(Protocol destProtocol, QString addrOSC, int midiChan, int midiNote, bool value) { btnAction(true, destProtocol, addrOSC, midiChan, midiNote, value); }
    void btnAction(bool condition, Protocol destProtocol, QString addrOSC, int midiChan, int midiNote, bool value);

    void sendFaderPos(Protocol destProtocol, int fader, int value);

    void processVPotLed(int chan, int value);

private:
    void selectChannel(Protocol p, int chan, bool value);
    void muteChannel(Protocol p, int chan, bool value);
    void soloChannel(Protocol p, int chan, bool value);
    void recChannel(Protocol p, int chan, bool value);
    void vPotSelectChannel(Protocol p, int chan, bool value);
    void assignTrack(Protocol p, bool value);
    void assignSend(Protocol p, bool value);
    void assignPanSurround(Protocol p, bool value);
    void assignPlugin(Protocol p, bool value);
    void assignEQ(Protocol p, bool value);
    void assignInstrument(Protocol p, bool value);
    void bankLeft(Protocol p);
    void bankRight(Protocol p);
    void channelLeft(Protocol p);
    void channelRight(Protocol p);
    void flip(Protocol p, bool value);
    void global(Protocol p, bool value);

private:
    Ui::Widget *    ui;

    QSettings       settings;

    QUdpSocket*     serverOSC;
    QUdpSocket*     clientOSC;

    MidiIO*         midiIO;

    QHostAddress    localAddress;
    QHostAddress    remoteAddress;
    int             remotePort;
    int             localPort;
};

#endif // WIDGET_H
