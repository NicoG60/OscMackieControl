#include "Translator.h"

Translator::Translator(QObject* parent) :
	QObject(parent)
{
	//Init Timecode var
	_timecode.fill(0, 10);

	//Init vu meter status
	_vuMeterStatus = new bool*[8];

	for(unsigned int i = 0; i < 8; i++)
		_vuMeterStatus[i] = new bool[12];

	for(unsigned int x = 0; x < 8; x++)
	{
		for(unsigned int y = 0; y < 12; y++)
			_vuMeterStatus[x][y] = false;
	}

	_touchedTimer.setInterval(100);
	_touchedTimer.setSingleShot(true);

	for(int i = 0; i < 9; i++)
		_touchedFaders[i] = false;

	//Connection slots
	connect(iOsc, &OscIO::oscMessage,				this, &Translator::readOSC);

	connect(iMidi, &MidiIO::midiNoteOn,				this, &Translator::midiNoteOn);
	connect(iMidi, &MidiIO::midiChannelPressure,	this, &Translator::midiChannelPressure);
	connect(iMidi, &MidiIO::midiPitchBend,			this, &Translator::midiPitchBend);
	connect(iMidi, &MidiIO::midiControler,			this, &Translator::midiControler);
	connect(iMidi, &MidiIO::midiSysex,				this, &Translator::midiSysex);

	connect(&_touchedTimer, &QTimer::timeout,		this, &Translator::resetFaderTouched);
}

Translator::~Translator()
{
	//Deleting vu meter status
	for(unsigned int i = 0; i < 8; i++)
		delete[] _vuMeterStatus[i];

	delete[] _vuMeterStatus;
}



//============================== MIDI SLOTS ====================================

void Translator::midiNoteOn(const uchar chan, const uchar note, const uchar vel)
{
	//Mackie Control use channel 0. If not, return
	if(chan > 0)
		return;

	//Check note and call appropriate function
	switch(note)
	{
	//0 - 7 : REC
	case Cm1:  //C-1  : Rec Ch 1
	case Cdm1: //C#-1 : Rec Ch 2
	case Dm1:  //D-1  : Rec Ch 3
	case Ddm1: //D#-1 : Rec Ch 4
	case Em1:  //E-1  : Rec Ch 5
	case Fm1:  //F-1  : Rec Ch 6
	case Fdm1: //F#-1 : Rec Ch 7
	case Gm1:  //G-1  : Rec Ch 8
		recChannel(OSC, note+1, vel>0);
		break;

	//8 - 15 : SOLO
	case Gdm1: //G#-1 : Solo Ch 1
	case Am1:  //A-1  : Solo Ch 2
	case Adm1: //A#-1 : Solo Ch 3
	case Bm1:  //B-1  : Solo Ch 4
	case C_0:  //C0   : Solo Ch 5
	case Cd_0: //C#0  : Solo Ch 6
	case D_0:  //D0   : Solo Ch 7
	case Dd_0: //D#0  : Solo Ch 8
		soloChannel(OSC, note-7, vel>0);
		break;

	//16 - 23 : MUTE
	case E_0:  //E0  : Mute Ch 1
	case F_0:  //F0  : Mute Ch 2
	case Fd_0: //F#0 : Mute Ch 3
	case G_0:  //G0  : Mute Ch 4
	case Gd_0: //G#0 : Mute Ch 5
	case A_0:  //A0  : Mute Ch 6
	case Ad_0: //A#0 : Mute Ch 7
	case B_0:  //B0  : Mute Ch 8
		muteChannel(OSC, note-15, vel>0);
		break;

	//24 - 31 : SELECT
	case C_1:  // C1  : Select Chn 1
	case Cd_1: // C#1 : Select Chn 2
	case D_1:  // D1  : Select Chn 3
	case Dd_1: // D#1 : Select Chn 4
	case E_1:  // E1  : Select Chn 5
	case F_1:  // F1  : Select Chn 6
	case Fd_1: // F#1 : Select Chn 7
	case G_1:  // G1  : Select Chn 8
		selectChannel(OSC, note-23, vel>0);
		break;

	//32 - 39 : VPOT
	case Gd_1: //G#1 : VPOT Select1
	case A_1:  //A1  : VPOT Select2
	case Ad_1: //A#1 : VPOT Select3
	case B_1:  //B1  : VPOT Select4
	case C_2:  //C2  : VPOT Select5
	case Cd_2: //C#2 : VPOT Select6
	case D_2:  //D2  : VPOT Select7
	case Dd_2: //D#2 : VPOT Select8
		vPotSelectChannel(OSC, note-31, vel>0);
		break;

	case E_2: // 40 E2 : Assign Track
		assignTrack(OSC, vel>0);
		break;

	case F_2: // 41 F2 : Assigne Send
		assignSend(OSC, vel>0);
		break;

	case Fd_2: // 42 F#2 : Assign Pan/Surround
		assignPanSurround(OSC, vel>0);
		break;

	case G_2: // 43 G2 : Assign Plug In
		assignPlugin(OSC, vel>0);
		break;

	case Gd_2: // 44 G#2 : Assign EQ
		assignEQ(OSC, vel>0);
		break;

	case A_2: // 45 A2 : Assign Instrument
		assignInstrument(OSC, vel>0);
		break;

	case Ad_2: // 46 A#2 :  Bank Left
		bankLeft(OSC, vel>0);
		break;

	case B_2: // 47 B2 : Bank Right
		bankRight(OSC, vel>0);
		break;

	case C_3: // 48 C3 : Channel Left
		channelLeft(OSC, vel>0);
		break;

	case Cd_3: // 49 C#3 :  Channel Right
		channelRight(OSC, vel>0);
		break;

	case D_3: // 50 D3 : Flip
		flip(OSC, vel>0);
		break;

	case Dd_3: // 51 D#3 : Global
		global(OSC, vel>0);
		break;

	case E_3: // 52 E3 : Display Name/Value
		nameValue(OSC, vel>0);
		break;

	case F_3: // 53 F3 : Display SMPTE/Beats
		smpteBeats(OSC, vel>0);
		break;

	// 54 - 61
	case Fd_3: // F#3 : F1
	case G_3:  // G3  : F2
	case Gd_3: // G#3 : F3
	case A_3:  // A3  : F4
	case Ad_3: // A#3 : F5
	case B_3:  // B3  : F6
	case C_4:  // C4  : F7
	case Cd_4: // C#4 : F8
		functionBtn(OSC, F_3 - note, vel>0);
		break;

	case D_4: // 62 D4 : Midi Tracks
		midiTracks(OSC, vel>0);
		break;

	case Dd_4: // 63 D#4 : Inputs
		inputs(OSC, vel>0);
		break;

	case E_4: // 64 E4 : AudioTracks
		audioTracks(OSC, vel>0);
		break;

	case F_4: // 65 F4 : Audio Instrument
		audioInstrument(OSC, vel>0);
		break;

	case Fd_4: // 66 F#4 : Aux
		aux(OSC, vel>0);
		break;

	case G_4: // 67 G4 : Bus
		busses(OSC, vel>0);
		break;

	case Gd_4: // 68 G#4 : Outputs
		outputs(OSC, vel>0);
		break;

	case A_4: // 69 A4 : User
		user(OSC, vel>0);
		break;

	case Ad_4: // 70 A#4 : Shift
		shift(OSC, vel>0);
		break;

	case B_4: // 71 B4 : Option
		option(OSC, vel>0);
		break;

	case C_5: // 72 C5 : Control
		control(OSC, vel>0);
		break;

	case Cd_5: // 73 C#5 : Alt
		alt(OSC, vel>0);
		break;

	case D_5: // 74 D5 : Read/Off
		readOff(OSC, vel>0);
		break;

	case Dd_5: // 75 D#5 : Write
		write(OSC, vel>0);
		break;

	case E_5: // 76 E5 : Trim
		trim(OSC, vel>0);
		break;

	case F_5: // 77 F5 : Touch
		touch(OSC, vel>0);
		break;

	case Fd_5: // 78 F#5 : Latch
		latch(OSC, vel>0);
		break;

	case G_5: // 79 G5 : Group
		group(OSC, vel>0);
		break;

	case Gd_5: // 80 G#5 : Save
		save(OSC, vel>0);
		break;

	case A_5: // 81 A5 : Undo
		undo(OSC, vel>0);
		break;

	case Ad_5: // 82 A#5 : Cancel
		cancel(OSC, vel>0);
		break;

	case B_5: // 83 B5 : Enter
		enter(OSC, vel>0);
		break;

	case C_6: // 84 C6 : Markers
		markers(OSC, vel>0);
		break;

	case Cd_6: // 85 C#6 : Nudge
		nudge(OSC, vel>0);
		break;

	case D_6: // 86 D6 : Cycle
		cycle(OSC, vel>0);
		break;

	case Dd_6: // 87 D#6 : Drop
		drop(OSC, vel>0);
		break;

	case E_6: // 88 E6 : Replace
		replace(OSC, vel>0);
		break;

	case F_6: // 89 F6 : Click
		click(OSC, vel>0);
		break;

	case Fd_6: // 90 F#6 : Solo
		solo(OSC, vel>0);
		break;

	case G_6: // 91 G6 : Rwd
		rewind(OSC, vel>0);
		break;

	case Gd_6: // 92 G#6 : Fwd
		forward(OSC, vel>0);
		break;

	case A_6: // 93 A6 : Stop
		stop(OSC, vel>0);
		break;

	case Ad_6: // 94 A#6 : Play
		play(OSC, vel>0);
		break;

	case B_6: // 95 B6 : Record
		rec(OSC, vel>0);
		break;

	case C_7: // 96 C7 : UP
		up(OSC, vel>0);
		break;

	case Cd_7: // 97 C#7 : DOWN
		down(OSC, vel>0);
		break;

	case D_7: // 98 D7 : Scrub
		scrub(OSC, vel>0);
		break;

	case Dd_7: // 99 D#7 : Zoom
		zoom(OSC, vel>0);
		break;

	case E_7: // 100 E7 : LEFT
		left(OSC, vel>0);
		break;

	case F_7: // 101 F7 : RIGHT
		right(OSC, vel>0);
		break;

		//=================================

	case F_8: // 113 F8 : SMPTE LED
		iOsc->sendOsc("/ledSMPTE", vel>0);
		break;

	case Fd_8: // 114 F#8 : BEATS LED
		iOsc->sendOsc("/ledBEATS", vel>0);
		break;
	}
}

void Translator::midiControler(const uchar chan, const uchar control, const uchar value)
{
	Q_UNUSED(chan)
	if(control >= 48 && control <= 55)		 //This case for vPot Leds
		processVPotLed(control-47, value);
	else if(control >= 64 && control <= 73)   //This case for timecode display
		processTimecode(control-63, value);
}

void Translator::midiChannelPressure(const uchar chan, const uchar value)
{
	if(chan > 0)
		return;

	//Vu meter status
	int track = (value & 0xF0) >> 4; //Channel id from 1 to 8
	int val = (value & 0x0F);		//Value from 0 to C

	processVuMeter(track, val);
}

void Translator::midiPitchBend(const uchar chan, const int value)
{
	if(chan < 8)
		sendFaderPos(OSC, chan+1, value); //Fader position
}

void Translator::midiSysex(const QByteArray &data)
{
	QByteArray starter;
	starter.append('\xF0');
	starter.append('\x00');
	starter.append('\x00');
	starter.append('\x66');
	starter.append('\x14');
	starter.append('\x12');

	if(data.left(6) == starter) //If the headers is corresponding
	{
		int start = data[6]+1;   //First character place
		int i = 0;

		while(data.at(7 + i) != '\xF7') //Sendind charracters 1 by 1 from "start" to the end of sysex message, increasing
		{
			QString addr = "/char" + QString::number(start+i);
			QString c = QString(data.at(7+i));
			iOsc->sendOsc(addr, c);
			i++;
		}
	}
}



//============================== OSC SLOTS ====================================

void Translator::readOSC(QByteArray raw)
{
	OscReader reader(&raw);
	OscMessage* msg = reader.getMessage();

	QString oscAddr = msg->getAddress(); //Reading osc address

	if(oscAddr.at(1).isNumber()) //Thats the page changing of TouchOSC
		return;

	int chan = oscAddr.right(1).toInt(); //if any, reading channel id
	int val = static_cast<int>(msg->getValue(0)->toFloat()); //Read osc value
	bool bVal = val != 0; //The boolean version of val

	//SPECIALS
	 CHECK_ADDR("/Jog",			scrubScrolling	(val));
	ECHECK_ADDR("/Fader",		sendFaderPos	(MIDI, chan, val));
	ECHECK_ADDR("/POT",			vPotScrolling	(chan, val));
	//Display
	ECHECK_ADDR("/NameValue",	nameValue		(MIDI, bVal));
	ECHECK_ADDR("/SmpteBeats",	smpteBeats		(MIDI, bVal));
	//Channel
	ECHECK_ADDR("/VPotSelect",	vPotSelectChannel(MIDI, chan, bVal));
	ECHECK_ADDR("/SEL",			selectChannel	(MIDI, chan, bVal));
	ECHECK_ADDR("/MUTE",		muteChannel		(MIDI, chan, bVal));
	ECHECK_ADDR("/SOLO",		soloChannel		(MIDI, chan, bVal));
	ECHECK_ADDR("/REC",			recChannel		(MIDI, chan, bVal));
	//Assignments
	ECHECK_ADDR("/Track",		assignTrack		(MIDI, bVal));
	ECHECK_ADDR("/Send",		assignSend		(MIDI, bVal));
	ECHECK_ADDR("/Pan",			assignPanSurround(MIDI, bVal));
	ECHECK_ADDR("/Plugin",		assignPlugin	(MIDI, bVal));
	ECHECK_ADDR("/EQ",			assignEQ		(MIDI, bVal));
	ECHECK_ADDR("/Instrument",	assignInstrument(MIDI, bVal));
	//Bank etc...
	ECHECK_ADDR("/BankL",		bankLeft		(MIDI, bVal));
	ECHECK_ADDR("/BankR",		bankRight		(MIDI, bVal));
	ECHECK_ADDR("/ChannelL",	channelLeft		(MIDI, bVal));
	ECHECK_ADDR("/ChannelR",	channelRight	(MIDI, bVal));
	ECHECK_ADDR("/Flip",		flip			(MIDI, bVal));
	ECHECK_ADDR("/Global",		global			(MIDI, bVal));
	//Functions
	ECHECK_ADDR("/F",			functionBtn		(MIDI, chan, bVal));
	ECHECK_ADDR("/MidiTracks",	midiTracks		(MIDI, bVal));
	ECHECK_ADDR("/Inputs",		inputs			(MIDI, bVal));
	ECHECK_ADDR("/AudioTracks",	audioTracks		(MIDI, bVal));
	ECHECK_ADDR("/AudioIntrument",audioInstrument(MIDI, bVal));
	ECHECK_ADDR("/Aux",			aux				(MIDI, bVal));
	ECHECK_ADDR("/Busses",		busses			(MIDI, bVal));
	ECHECK_ADDR("/Outputs",		outputs			(MIDI, bVal));
	ECHECK_ADDR("/User",		user			(MIDI, bVal));
	//Modifiers
	ECHECK_ADDR("/Shift",		shift			(MIDI, bVal));
	ECHECK_ADDR("/Option",		option			(MIDI, bVal));
	ECHECK_ADDR("/Control",		control			(MIDI, bVal));
	ECHECK_ADDR("/Alt",			alt				(MIDI, bVal));
	//Automation
	ECHECK_ADDR("/ReadOff",		readOff			(MIDI, bVal));
	ECHECK_ADDR("/Write",		write			(MIDI, bVal));
	ECHECK_ADDR("/Trim",		trim			(MIDI, bVal));
	ECHECK_ADDR("/Touch",		touch			(MIDI, bVal));
	ECHECK_ADDR("/Latch",		latch			(MIDI, bVal));
	ECHECK_ADDR("/Group",		group			(MIDI, bVal));
	//Utilities
	ECHECK_ADDR("/Save",		save			(MIDI, bVal));
	ECHECK_ADDR("/Undo",		undo			(MIDI, bVal));
	ECHECK_ADDR("/Cancel",		cancel			(MIDI, bVal));
	ECHECK_ADDR("/Enter",		enter			(MIDI, bVal));
	//Transport utilities
	ECHECK_ADDR("/Markers",		markers			(MIDI, bVal));
	ECHECK_ADDR("/Nudge",		nudge			(MIDI, bVal));
	ECHECK_ADDR("/Cycle",		cycle			(MIDI, bVal));
	ECHECK_ADDR("/Drop",		drop			(MIDI, bVal));
	ECHECK_ADDR("/Replace",		replace			(MIDI, bVal));
	ECHECK_ADDR("/Click",		click			(MIDI, bVal));
	ECHECK_ADDR("/Solo",		solo			(MIDI, bVal));
	//Transport
	ECHECK_ADDR("/Rewind",		rewind			(MIDI, bVal));
	ECHECK_ADDR("/Forward",		forward			(MIDI, bVal));
	ECHECK_ADDR("/Stop",		stop			(MIDI, bVal));
	ECHECK_ADDR("/Play",		play			(MIDI, bVal));
	ECHECK_ADDR("/Rec",			rec				(MIDI, bVal));
	//Zoom
	ECHECK_ADDR("/Up",			up				(MIDI, bVal));
	ECHECK_ADDR("/Down",		down			(MIDI, bVal));
	ECHECK_ADDR("/Scrub",		scrub			(MIDI, bVal));
	ECHECK_ADDR("/Zoom",		zoom			(MIDI, bVal));
	ECHECK_ADDR("/Left",		left			(MIDI, bVal));
	ECHECK_ADDR("/Right",		right			(MIDI, bVal));
}

void Translator::resetFaderTouched()
{
	for(int i = 0; i < 9; i++)
	{
		if(_touchedFaders[i])
		{
			_touchedFaders[i] = false;
			bangNote(0, Gd_7+i, 0);
		}
	}
}




//=============================== DISPLAY =====================================

void Translator::nameValue(Protocol p, bool value)
{
	btnAction(p, "/NameValue", 0, E_3, value);
}

void Translator::smpteBeats(Protocol p, bool value)
{
	btnAction(p, "/SmpteBeats", 0, F_3, value);
}

void Translator::processTimecode(int chan, int value)
{
	QString aff;

	if(value == 20) aff = " "; //20 = ASCII code for space
	else
	{
		bool dot = (value & 0x40); //Check if the dot is on

		aff = QString::number(value & 0x0F);

		if(dot) aff += ".";
	}

	iOsc->sendOsc("/Timecode" + QString::number(chan), aff);
}



//=============================== CHANNELS =====================================

void Translator::processVPotLed(int chan, int value)
{
	//Make the address string
	QString addr = "/POTRing" + QString::number(chan);

	//If the channel is what we expect
	if(chan >= 1 && chan <= 8)
	{
		//Init
		bool led1 = false;
		bool led2 = false;
		bool led3 = false;
		bool led4 = false;
		bool led5 = false;
		bool led6 = false;
		bool led7 = false;
		bool led8 = false;
		bool led9 = false;
		bool led10 = false;
		bool led11 = false;
		bool led12 = (value & 0x40) == 0x40;

		value = value & 0x3F;

		//Only one led mode
		if(value >= 0x00 && value <= 0x0B)
		{
			led1  = value == 0x01;
			led2  = value == 0x02;
			led3  = value == 0x03;
			led4  = value == 0x04;
			led5  = value == 0x05;
			led6  = value == 0x06;
			led7  = value == 0x07;
			led8  = value == 0x08;
			led9  = value == 0x09;
			led10 = value == 0x0A;
			led11 = value == 0x0B;
		}
		//Left / Right mode
		else if(value >= 0x10 && value <=0x1B)
		{
			led1  = value == 0x11;
			led2  = value == 0x11 || value == 0x12;
			led3  = value >= 0x11 && value <= 0x13;
			led4  = value >= 0x11 && value <= 0x14;
			led5  = value >= 0x11 && value <= 0x15;
			led6  = value >= 0x11 && value <= 0x1B;
			led7  = value >= 0x17 && value <= 0x1B;
			led8  = value >= 0x18 && value <= 0x1B;
			led9  = value >= 0x19 && value <= 0x1B;
			led10 = value == 0x1A || value == 0x1B;
			led11 = value == 0x1B;
		}
		// Min / Max mode
		else if(value >= 0x20 && value <= 0x2B)
		{
			led1  = value >= 0x21;
			led2  = value >= 0x22;
			led3  = value >= 0x23;
			led4  = value >= 0x24;
			led5  = value >= 0x25;
			led6  = value >= 0x26;
			led7  = value >= 0x27;
			led8  = value >= 0x28;
			led9  = value >= 0x29;
			led10 = value >= 0x2A;
			led11 = value >= 0x2B;
		}
		// Narrow / Wide mode
		else if((value >= 0x30 && value <=0x3B))
		{
			led1  = value >= 0x36;
			led2  = value >= 0x35;
			led3  = value >= 0x34;
			led4  = value >= 0x33;
			led5  = value >= 0x32;
			led6  = value >= 0x31;
			led7  = value >= 0x32;
			led8  = value >= 0x33;
			led9  = value >= 0x34;
			led10 = value >= 0x35;
			led11 = value >= 0x36;
		}

		//Sending all values
		iOsc->sendOsc(addr + QString::number(1),  led1  ? 1 : 0);
		iOsc->sendOsc(addr + QString::number(2),  led2  ? 1 : 0);
		iOsc->sendOsc(addr + QString::number(3),  led3  ? 1 : 0);
		iOsc->sendOsc(addr + QString::number(4),  led4  ? 1 : 0);
		iOsc->sendOsc(addr + QString::number(5),  led5  ? 1 : 0);
		iOsc->sendOsc(addr + QString::number(6),  led6  ? 1 : 0);
		iOsc->sendOsc(addr + QString::number(7),  led7  ? 1 : 0);
		iOsc->sendOsc(addr + QString::number(8),  led8  ? 1 : 0);
		iOsc->sendOsc(addr + QString::number(9),  led9  ? 1 : 0);
		iOsc->sendOsc(addr + QString::number(10), led10 ? 1 : 0);
		iOsc->sendOsc(addr + QString::number(11), led11 ? 1 : 0);

		iOsc->sendOsc("/POTLed" + QString::number(chan), led12 ? 1 : 0);
	}
}

void Translator::vPotScrolling(int chan, int value)
{
	uchar midiVal = 0;
	if(value == 0) midiVal = 65;
	if(value == 1) midiVal = 1;

	iMidi->sendControler(0, static_cast<uchar>(15+chan), midiVal);
}

void Translator::vPotSelectChannel(Protocol p, int chan, bool value)
{
	btnAction((chan >= 1 && chan <= 8), p, "/POTled" + QString::number(chan), 0, G_1 + chan, value);
}



void Translator::selectChannel(Protocol p, int chan, bool value)
{
	btnAction((chan >= 1 && chan <= 8), p, "/ledSEL" + QString::number(chan), 0, 23+chan, value);
}

void Translator::muteChannel(Protocol p, int chan, bool value)
{
	btnAction((chan >= 1 && chan <= 8), p, "/ledMUTE" + QString::number(chan), 0, 15+chan, value);
}

void Translator::soloChannel(Protocol p, int chan, bool value)
{
	btnAction((chan >= 1 && chan <= 8), p, "/ledSOLO" + QString::number(chan), 0, 7+chan, value);
}

void Translator::recChannel(Protocol p, int chan, bool value)
{
	btnAction((chan >= 1 && chan <= 8), p, "/ledREC" + QString::number(chan), 0, chan-1, value);
}


void Translator::sendFaderPos(Protocol destProtocol, int fader, int value)
{
	if(fader >= 1 && fader <= 9 && value <= 16383)
	{
		if(destProtocol == OSC)
			iOsc->sendOsc("/Fader" + QString::number(fader), value);
		else if(destProtocol == MIDI)
		{
			setFaderTouched(fader);
			iMidi->sendPitchBend(static_cast<uchar>(fader-1), value);
		}
	}
}


void Translator::processVuMeter(int chan, int value)
{
	bool* tabVuOld = _vuMeterStatus[chan];

	for(int i = 0; i < 12; i++)
	{
		bool status = (value >= i+1);
		if(tabVuOld[i] != status)
		{
			iOsc->sendOsc("/led" + QString::number(chan+1) + QString::number(i+1), status ? 1 : 0);
			tabVuOld[i] = status;
		}
	}
}



//=========================== MASTER / ASSIGNEMENT =============================

void Translator::assignTrack(Protocol p, bool value)
{
	btnAction(p, "/ledTrack", 0, E_2, value);
}

void Translator::assignSend(Protocol p, bool value)
{
	btnAction(p, "/ledSend", 0, F_2, value);
}

void Translator::assignPanSurround(Protocol p, bool value)
{
	btnAction(p, "/ledPan", 0, Fd_2, value);
}

void Translator::assignPlugin(Protocol p, bool value)
{
	btnAction(p, "/ledPlugin", 0, G_2, value);
}

void Translator::assignEQ(Protocol p, bool value)
{
	btnAction(p, "/ledEQ", 0, Gd_2, value);
}

void Translator::assignInstrument(Protocol p, bool value)
{
	btnAction(p, "/ledInstrument", 0, A_2, value);
}

void Translator::bankLeft(Protocol p, bool value)
{
	btnAction(p, "/BankL", 0, Ad_2, value);
}

void Translator::bankRight(Protocol p, bool value)
{
	btnAction(p, "/BankR", 0, B_2, value);
}

void Translator::channelLeft(Protocol p, bool value)
{
	btnAction(p, "/ChannelL", 0, C_3, value);
}

void Translator::channelRight(Protocol p, bool value)
{
	btnAction(p, "/ChannelR", 0, Cd_3, value);
}

void Translator::flip(Protocol p, bool value)
{
	btnAction(p, "/ledFlip", 0, D_3, value);
}

void Translator::global(Protocol p, bool value)
{
	btnAction(p, "/ledGlobal", 0, Dd_3, value);
}



//=============================== FUNCTIONS =====================================

void Translator::functionBtn(Protocol p, int nbre, bool value)
{
	btnAction(p, "/F" + QString::number(nbre), 0, F_3 + nbre, value);
}

void Translator::midiTracks(Protocol p, bool value)
{
	btnAction(p, "/MidiTracks", 0, D_4, value);
}

void Translator::inputs(Protocol p, bool value)
{
	btnAction(p, "/Inputs", 0, Dd_4, value);
}

void Translator::audioTracks(Protocol p, bool value)
{
	btnAction(p, "/AudioTracks", 0, E_4, value);
}

void Translator::audioInstrument(Protocol p, bool value)
{
	btnAction(p, "/AudioInstrument", 0, F_4, value);
}

void Translator::aux(Protocol p, bool value)
{
	btnAction(p, "/Aux", 0, Fd_4, value);
}

void Translator::busses(Protocol p, bool value)
{
	btnAction(p, "/Busses", 0, G_4, value);
}

void Translator::outputs(Protocol p, bool value)
{
	btnAction(p, "/Outputs", 0, Gd_4, value);
}

void Translator::user(Protocol p, bool value)
{
	btnAction(p, "/User", 0, A_4, value);
}

void Translator::shift(Protocol p, bool value)
{
	btnAction(p, "/Shift", 0, Ad_4, value);
}

void Translator::option(Protocol p, bool value)
{
	btnAction(p, "/Option", 0, B_4, value);
}

void Translator::control(Protocol p, bool value)
{
	btnAction(p, "/Control", 0, C_5, value);
}

void Translator::alt(Protocol p, bool value)
{
	btnAction(p, "/Alt", 0, Cd_5, value);
}

void Translator::readOff(Protocol p, bool value)
{
	btnAction(p, "/ledReadOff", 0, D_5, value);
}

void Translator::write(Protocol p, bool value)
{
	btnAction(p, "/ledWrite", 0, Dd_5, value);
}

void Translator::trim(Protocol p, bool value)
{
	btnAction(p, "/ledTrim", 0, E_5, value);
}

void Translator::touch(Protocol p, bool value)
{
	btnAction(p, "/ledTouch", 0, F_5, value);
}

void Translator::latch(Protocol p, bool value)
{
	btnAction(p, "/ledLatch", 0, Fd_5, value);
}

void Translator::group(Protocol p, bool value)
{
	btnAction(p, "/ledGroup", 0, G_5, value);
}

void Translator::save(Protocol p, bool value)
{
	btnAction(p, "/ledSave", 0, Gd_5, value);
}

void Translator::undo(Protocol p, bool value)
{
	btnAction(p, "/ledUndo", 0, A_5, value);
}

void Translator::cancel(Protocol p, bool value)
{
	btnAction(p, "/Cancel", 0, Ad_5, value);
}

void Translator::enter(Protocol p, bool value)
{
	btnAction(p, "/Enter", 0, B_5, value);
}

void Translator::markers(Protocol p, bool value)
{
	btnAction(p, "/ledMarkers", 0, C_6, value);
}

void Translator::nudge(Protocol p, bool value)
{
	btnAction(p, "/ledNudge", 0, Cd_6, value);
}

void Translator::cycle(Protocol p, bool value)
{
	btnAction(p, "/ledCycle", 0, D_6, value);
}

void Translator::drop(Protocol p, bool value)
{
	btnAction(p, "/ledDrop", 0, Dd_6, value);
}

void Translator::replace(Protocol p, bool value)
{
	btnAction(p, "/ledReplace", 0, E_6, value);
}

void Translator::click(Protocol p, bool value)
{
	btnAction(p, "/ledClick", 0, F_6, value);
}

void Translator::solo(Protocol p, bool value)
{
	btnAction(p, "/ledSolo", 0, Fd_6, value);
}



//=============================== TRANSPORT =====================================

void Translator::rewind(Protocol p, bool value)
{
	btnAction(p, "/ledRewind", 0, G_6, value);
}

void Translator::forward(Protocol p, bool value)
{
	btnAction(p, "/ledForward", 0, Gd_6, value);
}

void Translator::stop(Protocol p, bool value)
{
	btnAction(p, "/ledStop", 0, A_6, value);
}

void Translator::play(Protocol p, bool value)
{
	btnAction(p, "/ledPlay", 0, Ad_6, value);
}

void Translator::rec(Protocol p, bool value)
{
	btnAction(p, "/ledRec", 0, B_6, value);
}

void Translator::up(Protocol p, bool value)
{
	btnAction(p, "/Up", 0, C_7, value);
}

void Translator::down(Protocol p, bool value)
{
	btnAction(p, "/Down", 0, Cd_7, value);
}

void Translator::zoom(Protocol p, bool value)
{
	btnAction(p, "/ledZoom", 0, Dd_7, value);
}

void Translator::left(Protocol p, bool value)
{
	btnAction(p, "/Left", 0, E_7, value);
}

void Translator::right(Protocol p, bool value)
{
	btnAction(p, "/Right", 0, F_7, value);
}

void Translator::scrub(Protocol p, bool value)
{
	btnAction(p, "/ledScrub", 0, D_7, value);
}

void Translator::scrubScrolling(int value)
{
	uchar midiVal = 0;
	if(value == 0) midiVal = 65;
	if(value == 1) midiVal = 1;

	iMidi->sendControler(0, 60, midiVal);
}


//============================== UTILITIES ====================================

bool Translator::beginWith(QString addr, QString check)
{
	int n = check.size();

	return addr.left(n) == check;
}

void Translator::bangNote(int chan, int note, int vel)
{
	iMidi->sendNoteOn(static_cast<uchar>(chan),
					  static_cast<uchar>(note),
					  static_cast<uchar>(vel));

	iMidi->sendNoteOff(static_cast<uchar>(chan),
					   static_cast<uchar>(note),
					   0);
}

void Translator::btnAction(bool condition, Protocol destProtocol, QString addrOSC, int midiChan, int midiNote, bool value)
{
	if(condition)
	{
		if(destProtocol == OSC)
			iOsc->sendOsc(addrOSC, value?1:0);
		else if(destProtocol == MIDI)
			bangNote(midiChan, midiNote, value ? 127 : 0);
	}
}

void Translator::btnAction(Protocol destProtocol, QString addrOSC, int midiChan, int midiNote, bool value)
{
	btnAction(true, destProtocol, addrOSC, midiChan, midiNote, value);
}

void Translator::setFaderTouched(int chan)
{
	chan--;

	if(chan < 0 || chan > 8)
		return;

	if(!_touchedFaders[chan])
	{
		_touchedFaders[chan] = true;
		bangNote(0, Gd_7+chan, 255);
	}

	_touchedTimer.start();
}
