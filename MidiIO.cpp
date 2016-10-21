#include <QDebug>

#include "MidiIO.h"

MidiIO::MidiIO(QObject* parent) :
    QObject(parent),
    midiIn(new RtMidiIn()),
    midiOut(new RtMidiOut()),
    inPort(0),
    outPort(0),
    timer(new QTimer(this))
{
    connect(timer, SIGNAL(timeout()), this, SLOT(readMidiInput()));
    timer->start(0);
}

MidiIO::~MidiIO()
{
    midiIn->closePort();
    midiOut->closePort();

    timer->stop();

    delete midiIn;
    delete midiOut;
}

QStringList MidiIO::getAvailableInputPort()
{
    QStringList l;

    int nPort = midiIn->getPortCount();

    for(register int i = 0; i < nPort; i++)
        l.append(QString::fromStdString(midiIn->getPortName(i)));

    return l;
}

QStringList MidiIO::getAvailableOutputPort()
{
    QStringList l;

    int nPort = midiOut->getPortCount();

    for(register int i = 0; i < nPort; i++)
        l.append(QString::fromStdString(midiOut->getPortName(i)));

    return l;
}

bool MidiIO::connectInputPort(uint index)
{
    midiIn->closePort();
    bool r = index < midiIn->getPortCount();
    if(r)
    {
        try
        {
            midiIn->openPort(index);
        }
        catch(RtMidiError &e)
        {
            r = false;
            e.printMessage();
        }

        if(r)
        {
            inPort = index;
            midiIn->ignoreTypes(false, false, false);
        }
    }

    return r;
}

bool MidiIO::connectInputPort(QString name)
{
    QStringList l = getAvailableInputPort();
    bool r = l.contains(name);
    if(r)
    {
        r = connectInputPort(l.indexOf(name));
    }

    return r;
}

bool MidiIO::connectOutputPort(uint index)
{
    midiOut->closePort();
    bool r = index < midiOut->getPortCount();
    if(r)
    {
        try
        {
            midiOut->openPort(index);
        }
        catch(RtMidiError &e)
        {
            r = false;
            e.printMessage();
        }

        if(r) outPort = index;
    }

    return r;
}

bool MidiIO::connectOutputPort(QString name)
{
    QStringList l = getAvailableInputPort();
    bool r = l.contains(name);
    if(r)
    {
        r = connectOutputPort(l.indexOf(name));
    }

    return r;
}

QString MidiIO::getStatus()
{
    QString status;

    //---- MIDI IN
    status += "MIDI Input :\n";
    status += "Status : ";
    if(midiIn->isPortOpen())
    {
        status += "Ouvert\n";
        status += "Port : " + QString::fromStdString(midiIn->getPortName(inPort)) + "\n";
    }
    else
        status +="Fermé\n";

    status += "------------------------\n\n";

    //---- MIDI OUT
    status += "MIDI Output :\n";
    status += "Status : ";
    if(midiOut->isPortOpen())
    {
        status += "Ouvert\n";
        status += "Port : " + QString::fromStdString(midiOut->getPortName(outPort)) + "\n";
    }
    else
        status +="Fermé\n";

    status += "------------------------\n\n";

    return status;
}

void MidiIO::sendNoteOff(const int chan, const int note, const int vel)
{
    std::vector<unsigned char> msg;
    msg.push_back(MIDI_STATUS_NOTEOFF | (chan & MIDI_CHANNEL_MASK));
    msg.push_back(note);
    msg.push_back(vel);

    midiOut->sendMessage(&msg);
}

void MidiIO::sendNoteOn(const int chan, const int note, const int vel)
{
    std::vector<unsigned char> msg;
    msg.push_back(MIDI_STATUS_NOTEON | (chan & MIDI_CHANNEL_MASK));
    msg.push_back(note);
    msg.push_back(vel);

    midiOut->sendMessage(&msg);
}

void MidiIO::sendKeyPressure(const int chan, const int note, const int vel)
{
    std::vector<unsigned char> msg;
    msg.push_back(MIDI_STATUS_KEYPRESURE | (chan & MIDI_CHANNEL_MASK));
    msg.push_back(note);
    msg.push_back(vel);

    midiOut->sendMessage(&msg);
}

void MidiIO::sendControler(const int chan, const int control, const int value)
{
    std::vector<unsigned char> msg;
    msg.push_back(MIDI_STATUS_CONTROLCHANGE | (chan & MIDI_CHANNEL_MASK));
    msg.push_back(control);
    msg.push_back(value);

    midiOut->sendMessage(&msg);
}

void MidiIO::sendProgram(const int chan, const int program)
{
    std::vector<unsigned char> msg;
    msg.push_back(MIDI_STATUS_PROGRAMCHANGE | (chan & MIDI_CHANNEL_MASK));
    msg.push_back(program);

    midiOut->sendMessage(&msg);
}

void MidiIO::sendChannelPressure(const int chan, const int value)
{
    std::vector<unsigned char> msg;
    msg.push_back(MIDI_STATUS_CHANNELPRESSURE | (chan & MIDI_CHANNEL_MASK));
    msg.push_back(value);

    midiOut->sendMessage(&msg);
}

void MidiIO::sendPitchBend(const int chan, const int value)
{
    std::vector<unsigned char> msg;
    msg.push_back(MIDI_STATUS_PITCHBEND | (chan & MIDI_CHANNEL_MASK));
    msg.push_back(MIDI_LSB(value));
    msg.push_back(MIDI_MSB(value));

    midiOut->sendMessage(&msg);
}

void MidiIO::sendSysex(const QByteArray &data)
{
    std::vector<unsigned char> msg(data.begin(), data.end());
    midiOut->sendMessage(&msg);
}

void MidiIO::readMidiInput()
{
    std::vector<unsigned char> msg;

    midiIn->getMessage(&msg);

    if(msg.size() > 0)
    {
        int status = msg.at(0) & 0xf0;

        if(status == 0xF0)
        {
            QByteArray data(reinterpret_cast<const char*>(msg.data()), msg.size());
            emit midiSysex(data);
        }
        else
        {
            int chan = msg.at(0) & 0x0f;
            int data1 = msg.at(1);
            int data2;

            if(msg.size() == 3) data2 = msg.at(2);

            switch(status)
            {
            case MIDI_STATUS_NOTEON:
                emit midiNoteOn(chan, data1, data2);
                break;

            case MIDI_STATUS_NOTEOFF:
                emit midiNoteOff(chan, data1, data2);
                break;

            case MIDI_STATUS_KEYPRESURE:
                emit midiKeyPressure(chan, data1, data2);
                break;

            case MIDI_STATUS_CONTROLCHANGE:
                emit midiControler(chan, data1, data2);
                break;

            case MIDI_STATUS_PROGRAMCHANGE:
                emit midiProgram(chan, data1);
                break;

            case MIDI_STATUS_CHANNELPRESSURE:
                emit midiChannelPressure(chan, data1);
                break;

            case MIDI_STATUS_PITCHBEND:
                emit midiPitchBend(chan, data1+data2*0x80);
                break;
            }
        }
    }
}
