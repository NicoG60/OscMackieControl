#include "backend.h"

Backend::Backend(QOscInterface* osc, QMidi* midi, QObject *parent) :
    QObject(parent),
    _osc(osc),
    _midi(midi),
    _lcd(56*2, ' ')
{
    Q_ASSERT(_osc);
    Q_ASSERT(_midi);

    connect(_midi, &QMidi::midiNoteOn,
            this, &Backend::midiNoteOn);

    connect(_midi, &QMidi::midiControlChange,
            this, &Backend::midiControler);

    connect(_midi, &QMidi::midiChannelPressure,
            this, &Backend::midiChannelPressure);

    connect(_midi, &QMidi::midiPitchBend,
            this, &Backend::midiPitchBend);

    connect(_midi, &QMidi::midiSystemExclusive,
            this, &Backend::midiSysex);

    connect(_osc, &QOscInterface::messageReceived,
            this, &Backend::oscReceived);
}

QJsonObject Backend::dumpMapping()
{
    return _mapping.dumpJson();
}

void Backend::loadMapping(const QJsonObject &obj)
{
    _mapping.loadFromJson(obj);
    applyMapping();
}

void Backend::midiNoteOn(quint8 chan, quint8 note, quint8 vel)
{
    //Mackie Control use channel 0. If not, return
    if(chan > 0)
        return;

   auto it = _noteButtonMap.find(note);

   if(it == _noteButtonMap.end())
       return;

   sendButtonAction(it.value(), vel > 0);
}

void Backend::midiControler(quint8 chan, quint8 control, uchar value)
{
    Q_UNUSED(chan)
    if(control >= 48 && control <= 55)		 //This case for vPot Leds
        processVPotLed(control-48, value);
    else if(control >= 64 && control <= 73)   //This case for timecode display
        processTimecode(control-64, value);
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
        midiFader(chan, value); //Fader position
}

void Backend::midiSysex(const QByteArray& data)
{
    if(!data.startsWith("\x00\x00\x66\x14\x12"))
        return;

    sendLcdText(data.mid(5));
}

void Backend::oscReceived(const QOscMessage& msg)
{
    QRegularExpression faderRegexp(_mapping.faderBaseAddr + "\\d");
    QRegularExpression vpotRegexp(_mapping.VPOTSelect.addrIn + "\\d");

    QString p = msg.pattern();

    if(p == _mapping.jogBaseAddr)
    {
        processScrub(msg.toInt());
        return;
    }

    if(faderRegexp.match(p).hasMatch())
    {
        quint8 track = p.rightRef(1).toInt();
        processFaderPos(track, msg.toFloat());
        return;
    }

    if(vpotRegexp.match(p).hasMatch())
    {
        quint8 track = p.rightRef(1).toInt();
        processVPotScroll(track, msg.toInt());
        return;
    }

    auto it = _oscButtonMap.find(p);

    if(it == _oscButtonMap.end())
        return;

    sendButtonAction(it.value(), msg.toInt() != 0);
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
    auto& t = _tracks[track];

    if(!t.faderTouched)
    {
        t.faderTouched = true;
        bangNote(0, QMidi::GSharp_7 + track, 127);
        t.faderTimer.start();
    }
}

void Backend::resetFaderTouched(quint8 track)
{
    auto& t = _tracks[track];

    if(t.faderTouched)
    {
        t.faderTouched = false;
        bangNote(0, QMidi::GSharp_7 + track, 0);
    }
}

void Backend::processScrub(int value)
{
    quint8 midiVal = 0;

    switch(value)
    {
        case 0: midiVal = 65; break;
        case 1: midiVal = 1; break;
        default: break;
    }

    _midi->sendControlChange(0, 60, midiVal);
}

void Backend::processFaderPos(quint8 track, float pos)
{
    setFaderTouched(track);
    _midi->sendPitchBend(track, static_cast<quint16>(pos * 16383));
}

void Backend::processVPotScroll(quint8 track, int value)
{
    quint8 midiVal = 0;

    switch(value)
    {
        case 0: midiVal = 65; break;
        case 1: midiVal = 1; break;
        default: break;
    }

    _midi->sendControlChange(0, 16 + track, midiVal);
}

void Backend::processVPotLed(quint8 track, quint8 value)
{
    QString baseAddr = craftAddr(_mapping.vuMeterBaseAddr, track+1);
    QOscBundle b;

    b << QOscMessage(craftAddr(_mapping.vPotLedBaseAddr, track+1), (value & 0x40) ? 1 : 0);

    //Only one led mode
    if(value >= 0x00 && value <= 0x0B)
    {
        b << QOscMessage(craftAddr(baseAddr, 1), (value == 0x01) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 2), (value == 0x02) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 3), (value == 0x03) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 4), (value == 0x04) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 5), (value == 0x05) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 6), (value == 0x06) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 7), (value == 0x07) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 8), (value == 0x08) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 9), (value == 0x09) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 10), (value == 0x0A) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 11), (value == 0x0B) ? 1 : 0);
    }
    //Left / Right mode
    else if(value >= 0x10 && value <=0x1B)
    {
        b << QOscMessage(craftAddr(baseAddr, 1), (value == 0x11) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 2), (value == 0x11 || value == 0x12) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 3), (value >= 0x11 && value <= 0x13) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 4), (value >= 0x11 && value <= 0x14) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 5), (value >= 0x11 && value <= 0x15) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 6), (value >= 0x11 && value <= 0x1B) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 7), (value >= 0x17 && value <= 0x1B) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 8), (value >= 0x18 && value <= 0x1B) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 9), (value >= 0x19 && value <= 0x1B) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 10), (value == 0x1A || value == 0x1B) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 11), (value == 0x1B) ? 1 : 0);
    }
    // Min / Max mode
    else if(value >= 0x20 && value <= 0x2B)
    {
        b << QOscMessage(craftAddr(baseAddr, 1), (value >= 0x21) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 2), (value >= 0x22) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 3), (value >= 0x23) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 4), (value >= 0x24) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 5), (value >= 0x25) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 6), (value >= 0x26) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 7), (value >= 0x27) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 8), (value >= 0x28) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 9), (value >= 0x29) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 10), (value >= 0x2A) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 11), (value >= 0x2B) ? 1 : 0);
    }
    // Narrow / Wide mode
    else if((value >= 0x30 && value <=0x3B))
    {
        b << QOscMessage(craftAddr(baseAddr, 1), (value >= 0x36) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 2), (value >= 0x35) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 3), (value >= 0x34) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 4), (value >= 0x33) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 5), (value >= 0x32) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 6), (value >= 0x31) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 7), (value >= 0x32) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 8), (value >= 0x33) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 9), (value >= 0x34) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 10), (value >= 0x35) ? 1 : 0);
        b << QOscMessage(craftAddr(baseAddr, 11), (value >= 0x36) ? 1 : 0);
    }

    _osc->send(b);
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

    _osc->send(craftAddr(_mapping.timecodeBaseAddr, chan+1), aff);
}

void Backend::processVuMeter(quint8 track, quint8 value)
{
    auto& t = _tracks[track];

    QString baseAddr = craftAddr(_mapping.vuMeterBaseAddr, track+1);
    QOscBundle b;

    for(int i = 0; i < 12; i++)
    {
        bool status = (value >= i+1);
        if(t.vuMeter[i] != status)
        {
            b << QOscMessage(craftAddr(baseAddr, i+1), status ? 1 : 0);
            t.vuMeter[i] = status;
        }
    }

    if(!b.isEmpty())
        _osc->send(b);
}

void Backend::midiFader(quint8 track, quint16 value)
{
    _osc->send(craftAddr(_mapping.faderBaseAddr, track+1),
               value / 16383.0f);
}

void Backend::sendLcdText(const QByteArray& data)
{
    QOscBundle b;

    auto it = data.begin();

    int start = *it;
    int i = start;

    // Send individual characters
    for(++it; it != data.end(); ++it, ++i)
    {
        char c = *it;
        _lcd[i] = c;
        b << QOscMessage(craftAddr(_mapping.charBaseAddr, i+1), QString(1, c));
    }

    // Send each line
    if(start < 56)
        b << QOscMessage(craftAddr(_mapping.lcdLineBaseAddr, 1), _lcd.left(56));

    if(i >= 56)
        b << QOscMessage(craftAddr(_mapping.lcdLineBaseAddr, 2), _lcd.right(56));

    // do something to send each track individually
}

QString Backend::craftAddr(const QString& base, int id)
{
    static QString tmp(QStringLiteral("%1%2"));
    return tmp.arg(base).arg(id);
}


void Backend::applyMapping()
{
    for(int i = 0; i < 8; i++)
    {
        applyButtonTrack(_mapping.REC, i);
        applyButtonTrack(_mapping.MUTE, i);
        applyButtonTrack(_mapping.SOLO, i);
        applyButtonTrack(_mapping.SEL, i);
        applyButtonTrack(_mapping.VPOTSelect, i);
        applyButtonTrack(_mapping.Function, i);
    }

    for(auto& btn : _mapping.otherButtons)
        applyButton(btn);


}

void Backend::applyButtonTrack(const Mapping::ButtonControl& btn, int i)
{
    _noteButtonMap.insert(btn.note + i, craftAddr(btn.addrOut, i+1));
    _oscButtonMap.insert(craftAddr(btn.addrIn, i+1), btn.note + i);
}

void Backend::applyButton(const Mapping::ButtonControl& btn)
{
    _noteButtonMap.insert(btn.note, btn.addrOut);
    _oscButtonMap.insert(btn.addrIn, btn.note);
}
