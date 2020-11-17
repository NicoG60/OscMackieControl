#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QOsc>
#include <QMidi>
#include <array>

#define OSC_BUTTON_PRESSED(Name, Note) \
    inline void osc##Name(const QOscMessage& msg) { sendButtonAction(Note, msg.toInt() != 0); }

#define OSC_RANGE_BUTTON_PRESSED(Name, Note) \
    inline void osc##Name(const QOscMessage& msg) \
    { \
        quint8 c = msg.pattern().right(1).toInt()-1; \
        sendButtonAction(Note + c, msg.toInt() != 0); \
    }

#define MIDI_BUTTON_PRESSED(Note, Addr) \
    case Note: sendButtonAction(QStringLiteral(Addr), vel > 0); break;

#define MIDI_RANGE_BUTTON_PRESSED(Note, Addr) \
    case Note: \
    case Note+1: \
    case Note+2: \
    case Note+3: \
    case Note+4: \
    case Note+5: \
    case Note+6: \
    case Note+7: \
        sendButtonAction(QStringLiteral(Addr).arg(note-(Note-1)), vel > 0); \
        break;

class Backend : public QObject
{
    Q_OBJECT
public:
    struct TrackState
    {
        QTimer faderTimer;
        bool faderTouched = false;
        std::array<bool, 12> vuMeter = { false, false, false, false, false, false, false, false, false, false, false, false };
        std::array<bool, 12> vPotLed = { false, false, false, false, false, false, false, false, false, false, false, false };
    };

public:
    explicit Backend(QOscInterface* osc, QMidi* midi, QObject *parent = nullptr);

private slots: // Midi callbacks
    void midiNoteOn(quint8 chan, quint8 note, quint8 vel);
    void midiControler(quint8 chan, quint8 control, uchar value);
    void midiChannelPressure(quint8 chan, quint8 value);
    void midiPitchBend(quint8 chan, quint16 value);
    void midiSysex(const QByteArray& data);

private slots: // Osc Callbacks
    void oscJog(const QOscMessage& msg);
    void oscFader(const QOscMessage& msg);
    void oscPOT(const QOscMessage& msg);

    OSC_BUTTON_PRESSED(NameValue,  QMidi::E_3)
    OSC_BUTTON_PRESSED(SmpteBeats, QMidi::F_3)

    OSC_RANGE_BUTTON_PRESSED(VPotSelect, QMidi::FSharp_1)
    OSC_RANGE_BUTTON_PRESSED(SEL, QMidi::C_1)
    OSC_RANGE_BUTTON_PRESSED(MUTE, QMidi::E_0)
    OSC_RANGE_BUTTON_PRESSED(SOLO, QMidi::GSharp_m1)
    OSC_RANGE_BUTTON_PRESSED(REC, QMidi::C_m1)

    OSC_BUTTON_PRESSED(Track,      QMidi::E_2)
    OSC_BUTTON_PRESSED(Send,       QMidi::F_2)
    OSC_BUTTON_PRESSED(Pan,        QMidi::FSharp_2)
    OSC_BUTTON_PRESSED(Plugin,     QMidi::G_2)
    OSC_BUTTON_PRESSED(EQ,         QMidi::GSharp_2)
    OSC_BUTTON_PRESSED(Instrument, QMidi::A_2)

    OSC_BUTTON_PRESSED(BankL,    QMidi::ASharp_2)
    OSC_BUTTON_PRESSED(BankR,    QMidi::B_2)
    OSC_BUTTON_PRESSED(ChannelL, QMidi::C_3)
    OSC_BUTTON_PRESSED(ChannelR, QMidi::CSharp_3)
    OSC_BUTTON_PRESSED(Flip,     QMidi::D_3)
    OSC_BUTTON_PRESSED(Global,   QMidi::DSharp_3)

    OSC_RANGE_BUTTON_PRESSED(Func,      QMidi::E_3)
    OSC_BUTTON_PRESSED(MidiTracks,      QMidi::D_4)
    OSC_BUTTON_PRESSED(Inputs,          QMidi::DSharp_4)
    OSC_BUTTON_PRESSED(AudioTracks,     QMidi::E_4)
    OSC_BUTTON_PRESSED(AudioInstrument, QMidi::F_4)
    OSC_BUTTON_PRESSED(Aux,             QMidi::FSharp_4)
    OSC_BUTTON_PRESSED(Busses,          QMidi::G_4)
    OSC_BUTTON_PRESSED(Outputs,         QMidi::GSharp_4)
    OSC_BUTTON_PRESSED(cUser,           QMidi::A_4)

    OSC_BUTTON_PRESSED(Shift,   QMidi::ASharp_4)
    OSC_BUTTON_PRESSED(Option,  QMidi::B_4)
    OSC_BUTTON_PRESSED(Control, QMidi::C_5)
    OSC_BUTTON_PRESSED(Alt,     QMidi::CSharp_5)

    OSC_BUTTON_PRESSED(ReadOff, QMidi::D_5)
    OSC_BUTTON_PRESSED(Write,   QMidi::DSharp_5)
    OSC_BUTTON_PRESSED(Trim,    QMidi::E_5)
    OSC_BUTTON_PRESSED(Touch,   QMidi::F_5)
    OSC_BUTTON_PRESSED(Latch,   QMidi::FSharp_5)
    OSC_BUTTON_PRESSED(Group,   QMidi::G_5)

    OSC_BUTTON_PRESSED(Save,   QMidi::GSharp_5)
    OSC_BUTTON_PRESSED(Undo,   QMidi::A_5)
    OSC_BUTTON_PRESSED(Cancel, QMidi::ASharp_5)
    OSC_BUTTON_PRESSED(Enter,  QMidi::B_5)

    OSC_BUTTON_PRESSED(Markers, QMidi::C_6)
    OSC_BUTTON_PRESSED(Nudge,   QMidi::CSharp_6)
    OSC_BUTTON_PRESSED(Cycle,   QMidi::D_6)
    OSC_BUTTON_PRESSED(Drop,    QMidi::DSharp_6)
    OSC_BUTTON_PRESSED(Replace, QMidi::E_6)
    OSC_BUTTON_PRESSED(Click,   QMidi::F_6)
    OSC_BUTTON_PRESSED(Solo,    QMidi::FSharp_6)

    OSC_BUTTON_PRESSED(Rewind,  QMidi::G_6)
    OSC_BUTTON_PRESSED(Forward, QMidi::GSharp_6)
    OSC_BUTTON_PRESSED(Stop,    QMidi::A_6)
    OSC_BUTTON_PRESSED(Play,    QMidi::ASharp_6)
    OSC_BUTTON_PRESSED(Rec,     QMidi::B_6)

    OSC_BUTTON_PRESSED(Up,    QMidi::C_7)
    OSC_BUTTON_PRESSED(Down,  QMidi::CSharp_7)
    OSC_BUTTON_PRESSED(Scrub, QMidi::D_7)
    OSC_BUTTON_PRESSED(Zoom,  QMidi::DSharp_7)
    OSC_BUTTON_PRESSED(Left,  QMidi::E_7)
    OSC_BUTTON_PRESSED(Right, QMidi::F_7)

private slots: // ====== Helpers
    void bangNote(quint8 chan, quint8 note, quint8 vel);

    void sendButtonAction(quint8 note, bool pressed);
    void sendButtonAction(const QString& addr, bool pressed);

    void setFaderTouched(quint8 track);
    void resetFaderTouched(quint8 trackm1);

    void processVPotLed(quint8 track, quint8 value);
    void processTimecode(quint8 chan, quint8 value);
    void processVuMeter(quint8 track, quint8 value);

    void midiFader(quint8 track, quint16 value);

    void sendLcdText(const QByteArray& data);

signals:

private:
    QOscInterface* _osc;
    QMidi*         _midi;

    std::array<TrackState, 9> _tracks;
};

#endif // BACKEND_H
