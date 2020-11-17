#include "backend.h"

Backend::Backend(QOscInterface* osc, QMidi* midi, QObject *parent) :
    QObject(parent),
    _osc(osc),
    _midi(midi)
{
    Q_ASSERT(_osc);
    Q_ASSERT(_midi);
}

void Backend::midiNoteOn(quint8 chan, quint8 note, quint8 vel)
{
    //Mackie Control use channel 0. If not, return
    if(chan > 0)
        return;

    //Check note and call appropriate function
    switch(note)
    {
    MIDI_RANGE_BUTTON_PRESSED(QMidi::C_m1,      "/ledREC%1")
    MIDI_RANGE_BUTTON_PRESSED(QMidi::GSharp_m1, "/ledSOLO%1")
    MIDI_RANGE_BUTTON_PRESSED(QMidi::E_0,       "/ledMUTE%1")
    MIDI_RANGE_BUTTON_PRESSED(QMidi::C_1,       "/ledSEL%1")
    MIDI_RANGE_BUTTON_PRESSED(QMidi::GSharp_1,  "/POTled%1")

    MIDI_BUTTON_PRESSED(QMidi::E_2,      "/ledTrack")
    MIDI_BUTTON_PRESSED(QMidi::F_2,      "/ledSend")
    MIDI_BUTTON_PRESSED(QMidi::FSharp_2, "/ledPan")
    MIDI_BUTTON_PRESSED(QMidi::G_2,      "/ledPlugin")
    MIDI_BUTTON_PRESSED(QMidi::GSharp_2, "/ledEQ")
    MIDI_BUTTON_PRESSED(QMidi::A_2,      "/ledInstrument")

    MIDI_BUTTON_PRESSED(QMidi::ASharp_2, "/BankL")
    MIDI_BUTTON_PRESSED(QMidi::B_2,      "/BankR")
    MIDI_BUTTON_PRESSED(QMidi::C_3,      "/ChannelL")
    MIDI_BUTTON_PRESSED(QMidi::CSharp_3, "/ChannelR")
    MIDI_BUTTON_PRESSED(QMidi::D_3,      "/ledFlip")
    MIDI_BUTTON_PRESSED(QMidi::DSharp_3, "/ledGlobal")

    MIDI_BUTTON_PRESSED(QMidi::E_3, "/NameValue")
    MIDI_BUTTON_PRESSED(QMidi::F_3, "/SmpteBeats")

    MIDI_RANGE_BUTTON_PRESSED(QMidi::FSharp_3, "/F%1")

    MIDI_BUTTON_PRESSED(QMidi::D_4,      "/MidiTracks")
    MIDI_BUTTON_PRESSED(QMidi::DSharp_4, "/Inputs")
    MIDI_BUTTON_PRESSED(QMidi::E_4,      "/AudioTracks")
    MIDI_BUTTON_PRESSED(QMidi::F_4,      "/AudioInstrument")
    MIDI_BUTTON_PRESSED(QMidi::FSharp_4, "/Aux")
    MIDI_BUTTON_PRESSED(QMidi::G_4,      "/Busses")
    MIDI_BUTTON_PRESSED(QMidi::GSharp_4, "/Outputs")
    MIDI_BUTTON_PRESSED(QMidi::A_4,      "/User")

    MIDI_BUTTON_PRESSED(QMidi::ASharp_4, "/Shift")
    MIDI_BUTTON_PRESSED(QMidi::B_4,      "/Option")
    MIDI_BUTTON_PRESSED(QMidi::C_5,      "/Control")
    MIDI_BUTTON_PRESSED(QMidi::CSharp_5, "/Alt")

    MIDI_BUTTON_PRESSED(QMidi::D_5,      "/ledReadOff")
    MIDI_BUTTON_PRESSED(QMidi::DSharp_5, "/ledWrite")
    MIDI_BUTTON_PRESSED(QMidi::E_5,      "/ledTrim")
    MIDI_BUTTON_PRESSED(QMidi::F_5,      "/ledTouch")
    MIDI_BUTTON_PRESSED(QMidi::FSharp_5, "/ledLatch")
    MIDI_BUTTON_PRESSED(QMidi::G_5,      "/ledGroup");

    MIDI_BUTTON_PRESSED(QMidi::GSharp_5, "/ledSave")
    MIDI_BUTTON_PRESSED(QMidi::A_5,      "/ledUndo")
    MIDI_BUTTON_PRESSED(QMidi::ASharp_5, "/Cancel")
    MIDI_BUTTON_PRESSED(QMidi::B_5,      "/ledEnter")

    MIDI_BUTTON_PRESSED(QMidi::C_6,      "/ledMarkers")
    MIDI_BUTTON_PRESSED(QMidi::CSharp_6, "/ledNudge")
    MIDI_BUTTON_PRESSED(QMidi::D_6,      "/ledCycle")
    MIDI_BUTTON_PRESSED(QMidi::DSharp_6, "/ledDrop")
    MIDI_BUTTON_PRESSED(QMidi::E_6,      "/ledReplace")
    MIDI_BUTTON_PRESSED(QMidi::F_6,      "/ledClick")
    MIDI_BUTTON_PRESSED(QMidi::FSharp_6, "/ledSolo")
    MIDI_BUTTON_PRESSED(QMidi::G_6,      "/ledRewind")
    MIDI_BUTTON_PRESSED(QMidi::GSharp_6, "/ledForward")
    MIDI_BUTTON_PRESSED(QMidi::A_6,      "/ledStop")
    MIDI_BUTTON_PRESSED(QMidi::ASharp_6, "/ledPlay")
    MIDI_BUTTON_PRESSED(QMidi::B_6,      "/ledRec")

    MIDI_BUTTON_PRESSED(QMidi::C_7,      "/Up")
    MIDI_BUTTON_PRESSED(QMidi::CSharp_7, "/Down")
    MIDI_BUTTON_PRESSED(QMidi::D_7,      "/ledScrub")
    MIDI_BUTTON_PRESSED(QMidi::DSharp_7, "/ledZoom")
    MIDI_BUTTON_PRESSED(QMidi::E_7,      "/Left")
    MIDI_BUTTON_PRESSED(QMidi::F_7,      "/Right")

    MIDI_BUTTON_PRESSED(QMidi::F_8,      "/ledSMPTE")
    MIDI_BUTTON_PRESSED(QMidi::FSharp_8, "/ledBEATS")
    }
}

void Backend::midiControler(quint8 chan, quint8 control, uchar value)
{
    Q_UNUSED(chan)
    if(control >= 48 && control <= 55)		 //This case for vPot Leds
        processVPotLed(control-47, value);
    else if(control >= 64 && control <= 73)   //This case for timecode display
        processTimecode(control-63, value);
}

void Backend::midiChannelPressure(quint8 chan, quint8 value)
{
    if(chan > 0)
        return;

    //Vu meter status
    quint8 track = (value & 0xF0) >> 4; //Channel id from 1 to 8
    quint8 val = (value & 0x0F);		//Value from 0 to C

    processVuMeter(track, val);
}

void Backend::midiPitchBend(quint8 chan, quint16 value)
{
    if(chan < 8)
        midiFader(chan+1, value); //Fader position
}

void Backend::midiSysex(const QByteArray& data)
{
    if(!data.startsWith("\x00\x00\x00\x66\x14\x12"))
        return;

    sendLcdText(data.mid(7));
}

void Backend::oscJog(const QOscMessage& msg)
{

}

void Backend::oscFader(const QOscMessage& msg)
{

}

void Backend::oscPOT(const QOscMessage& msg)
{

}

void Backend::bangNote(quint8 chan, quint8 note, quint8 vel)
{
    _midi->sendNoteOn( chan, note, vel);
    _midi->sendNoteOff(chan, note, 0);
}

void Backend::sendButtonAction(quint8 note, bool pressed)
{
    bangNote(0, note, pressed ? 127 : 0);
}

void Backend::sendButtonAction(const QString& addr, bool pressed)
{
    _osc->send(addr, pressed ? 1 : 0);
}

void Backend::setFaderTouched(quint8 track)
{
    track--;
    auto& t = _tracks[track];

    if(!t.faderTouched)
    {
        t.faderTouched = true;
        bangNote(0, QMidi::GSharp_7 + track, 127);
        t.faderTimer.start();
    }
}

void Backend::resetFaderTouched(quint8 trackm1)
{
    auto& t = _tracks[trackm1];

    if(t.faderTouched)
    {
        t.faderTouched = false;
        bangNote(0, QMidi::GSharp_7 + trackm1, 0);
    }
}

void Backend::processVPotLed(quint8 track, quint8 value)
{

}

void Backend::processTimecode(quint8 chan, quint8 value)
{
    QString aff;

    if(value == 20) aff = " "; //20 = ASCII code for space
    else
    {
        bool dot = (value & 0x40); //Check if the dot is on

        aff = QString::number(value & 0x0F);

        if(dot) aff += ".";
    }

    _osc->send(QStringLiteral("/Timecode%1").arg(chan), aff);
}

void Backend::processVuMeter(quint8 track, quint8 value)
{

}

void Backend::midiFader(quint8 track, quint16 value)
{

}

void Backend::sendLcdText(const QByteArray& data)
{

}
