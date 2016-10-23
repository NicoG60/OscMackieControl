#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QDebug>

#include "MidiIO.h"
#include "OscIO.h"


class Translator : public QObject
{
    Q_OBJECT

public:
    enum Protocol {
        OSC,
        MIDI
    };

    Translator(OscIO* oscIO, MidiIO* midiIO, QObject* parent = 0);
    ~Translator();

private slots:
    void midiNoteOff        (const int chan, const int note, const int vel);
    void midiNoteOn         (const int chan, const int note, const int vel);
    void midiKeyPressure    (const int chan, const int note, const int vel);
    void midiControler      (const int chan, const int control, const int value);
    void midiProgram        (const int chan, const int program);
    void midiChannelPressure(const int chan, const int value);
    void midiPitchBend      (const int chan, const int value);
    void midiSysex          (const QByteArray &data);
    void midiSystemCommon   (const int status);
    void midiSystemRealtime (const int status);

    void readOSC            (OscMessage* msg);

private:
    bool isReady();
    void bangNote   (int chan, int note, int vel);
    void btnAction  (Protocol destProtocol, QString addrOSC, int midiChan, int midiNote, bool value) { btnAction(true, destProtocol, addrOSC, midiChan, midiNote, value); }
    void btnAction  (bool condition, Protocol destProtocol, QString addrOSC, int midiChan, int midiNote, bool value);

private:
    //================= Displays
    void nameValue      (Protocol p, bool value);
    void smpteBeats     (Protocol p, bool value);
    void processTimecode(int chan, int value);


    //================= Channels
    void processVPotLed     (int chan, int value);
    void vPotScrolling      (int chan, int value);
    void vPotSelectChannel  (Protocol p, int chan, bool value);

    void selectChannel      (Protocol p, int chan, bool value);
    void muteChannel        (Protocol p, int chan, bool value);
    void soloChannel        (Protocol p, int chan, bool value);
    void recChannel         (Protocol p, int chan, bool value);

    void sendFaderPos       (Protocol destProtocol, int fader, int value);

    void processVuMeter     (int chan, int value);


    //================= Master / Assignment
    //Assignment
    void assignTrack        (Protocol p, bool value);
    void assignSend         (Protocol p, bool value);
    void assignPanSurround  (Protocol p, bool value);
    void assignPlugin       (Protocol p, bool value);
    void assignEQ           (Protocol p, bool value);
    void assignInstrument   (Protocol p, bool value);

    //Bank / Channel /Flip
    void bankLeft           (Protocol p, bool value);
    void bankRight          (Protocol p, bool value);

    void channelLeft        (Protocol p, bool value);
    void channelRight       (Protocol p, bool value);

    void flip               (Protocol p, bool value);
    void global             (Protocol p, bool value);


    //================= Functions
    //F_ btn
    void functionBtn    (Protocol p, int nbre, bool value);

    //Global view btn
    void midiTracks     (Protocol p, bool value);
    void inputs         (Protocol p, bool value);
    void audioTracks    (Protocol p, bool value);
    void audioInstrument(Protocol p, bool value);
    void aux            (Protocol p, bool value);
    void busses         (Protocol p, bool value);
    void outputs        (Protocol p, bool value);
    void user           (Protocol p, bool value);

    //Modifiers btn
    void shift          (Protocol p, bool value);
    void option         (Protocol p, bool value);
    void control        (Protocol p, bool value);
    void alt            (Protocol p, bool value);

    //Automation btn
    void readOff        (Protocol p, bool value);
    void write          (Protocol p, bool value);
    void trim           (Protocol p, bool value);
    void touch          (Protocol p, bool value);
    void latch          (Protocol p, bool value);
    void group          (Protocol p, bool value);

    //Utilities btn
    void save           (Protocol p, bool value);
    void undo           (Protocol p, bool value);
    void cancel         (Protocol p, bool value);
    void enter          (Protocol p, bool value);

    //Transport btn
    void markers        (Protocol p, bool value);
    void nudge          (Protocol p, bool value);
    void cycle          (Protocol p, bool value);
    void drop           (Protocol p, bool value);
    void replace        (Protocol p, bool value);
    void click          (Protocol p, bool value);
    void solo           (Protocol p, bool value);

    //================= Transport
    void rewind         (Protocol p, bool value);
    void forward        (Protocol p, bool value);
    void stop           (Protocol p, bool value);
    void play           (Protocol p, bool value);
    void rec            (Protocol p, bool value);

    //Zoom
    void up             (Protocol p, bool value);
    void down           (Protocol p, bool value);
    void zoom           (Protocol p, bool value);
    void left           (Protocol p, bool value);
    void right          (Protocol p, bool value);

    //Jog
    void scrub          (Protocol p, bool value);
    void scrubScrolling (int value);

private:
    OscIO*          oscIO;

    MidiIO*         midiIO;

    QByteArray      timecode;

    bool**          vuMeterStatus;
};

#endif // TRANSLATOR_H
