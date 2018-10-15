#include <QDebug>

#include "MidiIO.h"

MidiIO* MidiIO::_instance = nullptr;

MidiIO::MidiIO(QObject* parent) :
	QObject(parent),
	_midiIn(new RtMidiIn()),
	_midiOut(new RtMidiOut()),
	_inPort(0),
	_outPort(0)
{
	_midiIn->ignoreTypes(false, false, false);
	_midiIn->setCallback(&MidiIO::readMidiInput, static_cast<void*>(this));
}

MidiIO::~MidiIO()
{
	_midiIn->closePort();
	_midiOut->closePort();

	delete _midiIn;
	delete _midiOut;
}

MidiIO* MidiIO::instance()
{
	if(!_instance)
		_instance = new MidiIO();

	return _instance;
}


QStringList MidiIO::getAvailableInputPort()
{
	return getAvailablePort(_midiIn);
}

QStringList MidiIO::getAvailableOutputPort()
{
	return getAvailablePort(_midiOut);
}



bool MidiIO::connectInputPort(uint index)
{
	return connectPort(_midiIn, index, _inPort);
}

bool MidiIO::connectInputPort(QString name)
{
	return connectPort(_midiIn, name, _inPort);
}

bool MidiIO::connectOutputPort(uint index)
{
	return connectPort(_midiOut, index, _outPort);
}

bool MidiIO::connectOutputPort(QString name)
{
	return connectPort(_midiOut, name, _outPort);
}



QString MidiIO::getStatus()
{
	QString status;

	//---- MIDI IN
	status += "MIDI Input :\n";
	status += "Status : ";
	if(_midiIn->isPortOpen())
	{
		status += "Opened\n";
		status += "Port : " + QString::fromStdString(_midiIn->getPortName(_inPort)) + "\n";
	}
	else
		status +="Closed\n";

	status += "------------------------\n\n";

	//---- MIDI OUT
	status += "MIDI Output :\n";
	status += "Status : ";
	if(_midiOut->isPortOpen())
	{
		status += "Opened\n";
		status += "Port : " + QString::fromStdString(_midiOut->getPortName(_outPort)) + "\n";
	}
	else
		status +="Closed\n";

	status += "------------------------\n\n";

	return status;
}



void MidiIO::sendNoteOff(const uchar chan, const uchar note, const uchar vel)
{
	send(MIDI_STATUS_NOTEOFF, chan, note, vel);
}

void MidiIO::sendNoteOn(const uchar chan, const uchar note, const uchar vel)
{
	send(MIDI_STATUS_NOTEON, chan, note, vel);
}

void MidiIO::sendKeyPressure(const uchar chan, const uchar note, const uchar vel)
{
	send(MIDI_STATUS_KEYPRESURE, chan, note, vel);
}

void MidiIO::sendControler(const uchar chan, const uchar control, const uchar value)
{
	send(MIDI_STATUS_CONTROLCHANGE, chan, control, value);
}

void MidiIO::sendProgram(const uchar chan, const uchar program)
{
	send(MIDI_STATUS_PROGRAMCHANGE, chan, program);
}

void MidiIO::sendChannelPressure(const uchar chan, const uchar value)
{
	send(MIDI_STATUS_CHANNELPRESSURE, chan, value);
}

void MidiIO::sendPitchBend(const uchar chan, const int value)
{
	send(MIDI_STATUS_PITCHBEND, chan, MIDI_LSB(value), MIDI_MSB(value));
}

void MidiIO::sendSysex(const QByteArray &data)
{
	if(!_midiOut->isPortOpen())
		return;

	std::vector<unsigned char> msg(data.begin(), data.end());
	_midiOut->sendMessage(&msg);
}



void MidiIO::readMidiInput(double timestamp, std::vector<unsigned char> *msg, void *data)
{
	Q_UNUSED(timestamp)

	MidiIO* _this = static_cast<MidiIO*>(data);

	if(msg->size() < 2)
		return;

	int status = msg->at(0) & 0xF0;

	if(status == 0xF0)
	{
		QByteArray data(reinterpret_cast<const char*>(msg->data()),
						static_cast<int>(msg->size()));
		_this->midiSysex(data);
		return;
	}

	uchar chan = msg->at(0) & 0x0F;
	uchar data1 = msg->at(1);
	uchar data2 = 0;

	if(msg->size() == 3)
		data2 = msg->at(2);

	switch(status)
	{
	case MIDI_STATUS_NOTEON:
		_this->midiNoteOn(chan, data1, data2);
		break;

	case MIDI_STATUS_NOTEOFF:
		_this->midiNoteOff(chan, data1, data2);
		break;

	case MIDI_STATUS_KEYPRESURE:
		_this->midiKeyPressure(chan, data1, data2);
		break;

	case MIDI_STATUS_CONTROLCHANGE:
		_this->midiControler(chan, data1, data2);
		break;

	case MIDI_STATUS_PROGRAMCHANGE:
		_this->midiProgram(chan, data1);
		break;

	case MIDI_STATUS_CHANNELPRESSURE:
		_this->midiChannelPressure(chan, data1);
		break;

	case MIDI_STATUS_PITCHBEND:
		_this->midiPitchBend(chan, data1+data2*0x80);
		break;
	}
}



QStringList MidiIO::getAvailablePort(RtMidi* port)
{
	QStringList l;

	uint nPort = port->getPortCount();

	for(uint i = 0; i < nPort; i++)
	{
		QString name = QString::fromStdString(port->getPortName(i));
		l.append(name);
	}

	return l;
}

bool MidiIO::connectPort(RtMidi* port, uint index, uint& intPort)
{
	port->closePort();
	bool r = index < port->getPortCount();
	if(r)
	{
		try
		{
			port->openPort(index);
			intPort = index;
		}
		catch(RtMidiError &e)
		{
			r = false;
			e.printMessage();
		}
	}

	return r;
}

bool MidiIO::connectPort(RtMidi* port, QString name, uint& intPort)
{
	QStringList l = getAvailablePort(port);
	bool r = l.contains(name);
	if(r)
		r = connectPort(port, static_cast<uint>(l.indexOf(name)), intPort);

	return r;
}

void MidiIO::send(const uchar type, const uchar chan, const uchar data1)
{
	if(!_midiOut->isPortOpen())
		return;

	std::vector<unsigned char> msg;
	msg.push_back(type | (chan & MIDI_CHANNEL_MASK));
	msg.push_back(data1);

	_midiOut->sendMessage(&msg);
}

void MidiIO::send(const uchar type, const uchar chan, const uchar data1, const uchar data2)
{
	if(!_midiOut->isPortOpen())
		return;

	std::vector<unsigned char> msg;
	msg.push_back(type | (chan & MIDI_CHANNEL_MASK));
	msg.push_back(data1);
	msg.push_back(data2);

	_midiOut->sendMessage(&msg);
}
