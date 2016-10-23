#include "Translator.h"

/**
 * Constructor of the translator object.
 * @brief Translator::Translator
 * @param parent QObject parent
 * @param oscIO : oscIO Object pointer, must be valid, translator do not take the ownership
 * @param midiIO : midiIO Object pointer, must be valid, translator do not take the ownership
 */
Translator::Translator(OscIO *oscIO, MidiIO *midiIO, QObject* parent) :
    QObject(parent),
    oscIO(oscIO),
    midiIO(midiIO)
{
    //Init Timecode var
    timecode.fill(0, 10);

    //Init vu meter status
    vuMeterStatus = new bool*[8];

    for(register unsigned int i = 0; i < 8; i++)
        vuMeterStatus[i] = new bool[12];

    for(register unsigned int x = 0; x < 8; x++)
    {
        for(register unsigned int y = 0; y < 12; y++)
            vuMeterStatus[x][y] = false;
    }

    //Connection slots
    connect(oscIO, SIGNAL(oscMessage(OscMessage*)), this, SLOT(readOSC(OscMessage*)));

    connect(midiIO, SIGNAL(midiNoteOn(int,int,int)), this, SLOT(midiNoteOn(int,int,int)));
    connect(midiIO, SIGNAL(midiNoteOff(int,int,int)), this, SLOT(midiNoteOff(int,int,int)));
    connect(midiIO, SIGNAL(midiKeyPressure(int,int,int)), this, SLOT(midiKeyPressure(int,int,int)));
    connect(midiIO, SIGNAL(midiChannelPressure(int,int)), this, SLOT(midiChannelPressure(int,int)));
    connect(midiIO, SIGNAL(midiPitchBend(int,int)), this, SLOT(midiPitchBend(int,int)));
    connect(midiIO, SIGNAL(midiControler(int,int,int)), this, SLOT(midiControler(int,int,int)));
    connect(midiIO, SIGNAL(midiProgram(int,int)), this, SLOT(midiProgram(int,int)));
    connect(midiIO, SIGNAL(midiSysex(QByteArray)), this, SLOT(midiSysex(QByteArray)));
}

/**
 * @brief Translator::~Translator
 */
Translator::~Translator()
{
    //Deleting vu meter status
    for(register unsigned int i = 0; i < 8; i++)
        delete[] vuMeterStatus[i];

    delete[] vuMeterStatus;
}



//============================== MIDI SLOTS ====================================
/**
 * Called when a midi Note Off is recieve but not use in this case.
 * @brief Translator::midiNoteOff
 * @param chan
 * @param note
 * @param vel
 */
void Translator::midiNoteOff(const int chan, const int note, const int vel)
{
    //Unused function
    Q_UNUSED(chan)
    Q_UNUSED(note)
    Q_UNUSED(vel)
}

/**
 * Called when a midi Note On is recieve. routing to the right function to call depending of the note.
 * @brief Translator::midiNoteOn
 * @param chan
 * @param note
 * @param vel
 */
void Translator::midiNoteOn(const int chan, const int note, const int vel)
{
    //Check note and call appropriate function
    if(chan == 0)
    {
        switch(note)
        {
        //0 - 7
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

        //8 - 15
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

        //16 - 23
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

        //24 - 31
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

        //32 - 39
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
            oscIO->sendOsc("/ledSMPTE", vel>0);
            break;

        case Fd_8: // 114 F#8 : BEATS LED
            oscIO->sendOsc("/ledBEATS", vel>0);
            break;
        }
    }
}

/**
 * Called when a midi Key After Touch is recieved, not use in this case.
 * @brief Translator::midiKeyPressure
 * @param chan
 * @param note
 * @param vel
 */
void Translator::midiKeyPressure(const int chan, const int note, const int vel)
{
    //Unused function
    Q_UNUSED(chan)
    Q_UNUSED(note)
    Q_UNUSED(vel)
}

/**
 * Called when a midi Control Change is recieved, routing to the right function to call depending of the control id : vPot Leds or timecode display
 * @brief Translator::midiControler
 * @param chan
 * @param control
 * @param value
 */
void Translator::midiControler(const int chan, const int control, const int value)
{
    if(chan == 0 && control >= 48 && control <= 55)         //This case for vPot Leds
        processVPotLed(control-47, value);
    else if(chan == 15 && control >= 64 && control <= 73)   //This case for timecode display
        processTimecode(control-63, value);
}

/**
 * Called when a midi Program Change is recieved, not use in this case.
 * @brief Translator::midiProgram
 * @param chan
 * @param program
 */
void Translator::midiProgram(const int chan, const int program)
{
    //Unused function
    Q_UNUSED(chan)
    Q_UNUSED(program)
}

/**
 * Called when a midi Channel After Touch is recieved, only used for VU Meter.
 * @brief Translator::midiChannelPressure
 * @param chan
 * @param value
 */
void Translator::midiChannelPressure(const int chan, const int value)
{
    if(chan == 0)
    {
        //Vu meter status
        int track = (value & 0xF0) >> 4; //Channel id from 1 to 8
        int val = (value & 0x0F);        //Value from 0 to C

        processVuMeter(track, val);
    }
}

/**
 * Called when a midi Pitch Bend is recieved, only used for fader position
 * @brief Translator::midiPitchBend
 * @param chan
 * @param value
 */
void Translator::midiPitchBend(const int chan, const int value)
{
    if(chan >= 0 && chan <= 8) sendFaderPos(OSC, chan+1, value); //Fader position
}

/**
 * Called when a midi System Exclusive is recieved, only for LCD display
 * @brief Translator::midiSysex
 * @param data
 */
void Translator::midiSysex(const QByteArray &data)
{
    if(data.at(0) == (char)0xF0 &&
       data.at(1) == (char)0x00 &&
       data.at(2) == (char)0x00 &&
       data.at(3) == (char)0x66 &&
       data.at(4) == (char)0x14 &&
       data.at(5) == (char)0x12) //If the headers is corresponding
    {
        int start = data[6]+1;   //First character place
        int i = 0;

        while(data.at(7 + i) != (char)0xF7) //Sendind charracters 1 by 1 from "start" to the end of sysex message, increasing
        {
            oscIO->sendOsc("/char" + QString::number(start+i), QString(data.at(7+i)));
            i++;
        }
    }
}

/**
 * @brief Translator::midiSystemCommon
 * @param status
 */
void Translator::midiSystemCommon(const int status)
{
    //Unused function
    Q_UNUSED(status)
}

/**
 * @brief Translator::midiSystemRealtime
 * @param status
 */
void Translator::midiSystemRealtime(const int status)
{
    //Unused function
    Q_UNUSED(status)
}



//============================== OSC SLOTS ====================================

/**
 * Called when an osc message is recieved, routing to the right function to call depending of the address.
 * @brief Translator::readOSC
 * @param msg
 */
void Translator::readOSC(OscMessage* msg)
{
    QString oscAddr = msg->getAddress(); //Reading osc address
    int chan = oscAddr.right(1).toInt(); //if any, reading channel id
    float val = 0; //Read osc value

    if(oscAddr != "/1" && oscAddr != "/2")
        val = msg->getValue(0)->toFloat();

    //Checing if it is special osc value (like jog, vPots, faders...)
    if      (oscAddr          == "/Jog")            scrubScrolling      (static_cast<int>(val));
    else if (oscAddr.left(6)  == "/fader")          sendFaderPos        (MIDI, chan, static_cast<int>(val));
    else if (oscAddr.left(4)  == "/POT")            vPotScrolling       (chan, static_cast<int>(val));
    //Display
    else if (oscAddr          == "/NameValue")      nameValue           (MIDI, val!=0);
    else if (oscAddr          == "/SmpteBeats")     smpteBeats          (MIDI, val!=0);
    //Channels
    else if (oscAddr.left(11) == "/VPotSelect")     vPotSelectChannel   (MIDI, chan, val!=0);
    else if (oscAddr.left(4)  == "/SEL")            selectChannel       (MIDI, chan, val!=0);
    else if (oscAddr.left(5)  == "/MUTE")           muteChannel         (MIDI, chan, val!=0);
    else if (oscAddr.left(5)  == "/SOLO")           soloChannel         (MIDI, chan, val!=0);
    else if (oscAddr.left(4)  == "/REC")            recChannel          (MIDI, chan, val!=0);
    //Assignments
    else if (oscAddr          == "/Track")          assignTrack         (MIDI, val!=0);
    else if (oscAddr          == "/Send")           assignSend          (MIDI, val!=0);
    else if (oscAddr          == "/Pan")            assignPanSurround   (MIDI, val!=0);
    else if (oscAddr          == "/Plugin")         assignPlugin        (MIDI, val!=0);
    else if (oscAddr          == "/EQ")             assignEQ            (MIDI, val!=0);
    else if (oscAddr          == "/Instrument")     assignInstrument    (MIDI, val!=0);
    //Bank etc...
    else if (oscAddr          == "/BankL")          bankLeft            (MIDI, val!=0);
    else if (oscAddr          == "/BankR")          bankRight           (MIDI, val!=0);
    else if (oscAddr          == "/ChannelL")       channelLeft         (MIDI, val!=0);
    else if (oscAddr          == "/ChannelR")       channelRight        (MIDI, val!=0);
    else if (oscAddr          == "/Flip")           flip                (MIDI, val!=0);
    else if (oscAddr          == "/Global")         global              (MIDI, val!=0);
    //Functions
    else if (oscAddr.left(2)  == "/F")              functionBtn         (MIDI, chan, val!=0);
    else if (oscAddr          == "/MidiTracks")     midiTracks          (MIDI, val!=0);
    else if (oscAddr          == "/Inputs")         inputs              (MIDI, val!=0);
    else if (oscAddr          == "/AudioTracks")    audioTracks         (MIDI, val!=0);
    else if (oscAddr          == "/AudioInstrument")audioInstrument     (MIDI, val!=0);
    else if (oscAddr          == "/Aux")            aux                 (MIDI, val!=0);
    else if (oscAddr          == "/Busses")         busses              (MIDI, val!=0);
    else if (oscAddr          == "/Outputs")        outputs             (MIDI, val!=0);
    else if (oscAddr          == "/User")           user                (MIDI, val!=0);
    //Modifiers
    else if (oscAddr          == "/Shift")          shift               (MIDI, val!=0);
    else if (oscAddr          == "/Option")         option              (MIDI, val!=0);
    else if (oscAddr          == "/Control")        control             (MIDI, val!=0);
    else if (oscAddr          == "/Alt")            alt                 (MIDI, val!=0);
    //Automation
    else if (oscAddr          == "/ReadOff")        readOff             (MIDI, val!=0);
    else if (oscAddr          == "/Write")          write               (MIDI, val!=0);
    else if (oscAddr          == "/Trim")           trim                (MIDI, val!=0);
    else if (oscAddr          == "/Touch")          touch               (MIDI, val!=0);
    else if (oscAddr          == "/Latch")          latch               (MIDI, val!=0);
    else if (oscAddr          == "/Group")          group               (MIDI, val!=0);
    //Utilities
    else if (oscAddr          == "/Save")           save                (MIDI, val!=0);
    else if (oscAddr          == "/Undo")           undo                (MIDI, val!=0);
    else if (oscAddr          == "/Cancel")         cancel              (MIDI, val!=0);
    else if (oscAddr          == "/Enter")          enter               (MIDI, val!=0);
    //Transport utilities
    else if (oscAddr          == "/Markers")        markers             (MIDI, val!=0);
    else if (oscAddr          == "/Nudge")          nudge               (MIDI, val!=0);
    else if (oscAddr          == "/Cycle")          cycle               (MIDI, val!=0);
    else if (oscAddr          == "/Drop")           drop                (MIDI, val!=0);
    else if (oscAddr          == "/Replace")        replace             (MIDI, val!=0);
    else if (oscAddr          == "/Click")          click               (MIDI, val!=0);
    else if (oscAddr          == "/Solo")           solo                (MIDI, val!=0);
    //Transport
    else if (oscAddr          == "/Rewind")         rewind              (MIDI, val!=0);
    else if (oscAddr          == "/Forward")        forward             (MIDI, val!=0);
    else if (oscAddr          == "/Stop")           stop                (MIDI, val!=0);
    else if (oscAddr          == "/Play")           play                (MIDI, val!=0);
    else if (oscAddr          == "/Rec")            rec                 (MIDI, val!=0);
    //Zoom
    else if (oscAddr          == "/Up")             up                  (MIDI, val!=0);
    else if (oscAddr          == "/Down")           down                (MIDI, val!=0);
    else if (oscAddr          == "/Scrub")          scrub               (MIDI, val!=0);
    else if (oscAddr          == "/Zoom")           zoom                (MIDI, val!=0);
    else if (oscAddr          == "/Left")           left                (MIDI, val!=0);
    else if (oscAddr          == "/Right")          right               (MIDI, val!=0);
}



//============================== UTILITIES ====================================

/**
 * Return true if all IO are ready to communicate.
 * @brief Translator::isReady
 * @return
 */
bool Translator::isReady()
{
    return oscIO->isOpen() && midiIO->isOpen();
}

/**
 * Send instant note On the note Off. Juste banging the note.
 * @brief Translator::bangNote
 * @param chan
 * @param note
 * @param vel
 */
void Translator::bangNote(int chan, int note, int vel)
{
    midiIO->sendNoteOn(chan, note, vel);
    midiIO->sendNoteOff(chan, note, 0);
}

/**
 * General case of an action on a button.
 * @brief Translator::btnAction
 * @param condition : condition to send data through protocol
 * @param destProtocol : the destination protocol where data will be sent
 * @param addrOSC : the corresponding OSC address
 * @param midiChan : the corresponding midi channel
 * @param midiNote : the corresponding midi note
 * @param value : true : btn down, false btn up
 */
void Translator::btnAction(bool condition, Protocol destProtocol, QString addrOSC, int midiChan, int midiNote, bool value)
{
    if(isReady() && condition)
    {
        if(destProtocol == OSC)
            oscIO->sendOsc(addrOSC, value?1:0);
        else if(destProtocol == MIDI)
            bangNote(midiChan, midiNote, value ? 127 : 0);
    }
}



//=============================== DISPLAY =====================================

/**
 * send Name / Value message through protocol
 * @brief Translator::nameValue
 * @param p
 */
void Translator::nameValue(Protocol p, bool value)
{
    btnAction(p, "/NameValue", 0, E_3, value);
}

/**
 * send SMPTE / BEATS message through protocol
 * @brief Translator::smpteBeats
 * @param p
 */
void Translator::smpteBeats(Protocol p, bool value)
{
    btnAction(p, "/SmpteBeats", 0, F_3, value);
}

/**
 * Analyze timecode midi data and send OSC well formated timecode data
 * @brief Translator::processTimecode
 * @param chan
 * @param value
 */
void Translator::processTimecode(int chan, int value)
{
    QString aff;

    if(value == 20) aff = " ";
    else
    {
        bool dot = (value & 0x40) == 0x40;

        aff = QString::number(value & 0x0F);

        if(dot) aff += ".";
    }

    oscIO->sendOsc("/Timecode" + QString::number(chan), aff);
}



//=============================== CHANNELS =====================================

/**
 * Analyze the midi data for the vPot Leds and send Osc well formed leds datas.
 * @brief Translator::processVPotLed
 * @param chan
 * @param value
 */
void Translator::processVPotLed(int chan, int value)
{
    QString addr = "/POTRing" + QString::number(chan);
    if(chan >= 1 && chan <= 8)
    {
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

        oscIO->sendOsc(addr + QString::number(1),  led1  ? 1 : 0);
        oscIO->sendOsc(addr + QString::number(2),  led2  ? 1 : 0);
        oscIO->sendOsc(addr + QString::number(3),  led3  ? 1 : 0);
        oscIO->sendOsc(addr + QString::number(4),  led4  ? 1 : 0);
        oscIO->sendOsc(addr + QString::number(5),  led5  ? 1 : 0);
        oscIO->sendOsc(addr + QString::number(6),  led6  ? 1 : 0);
        oscIO->sendOsc(addr + QString::number(7),  led7  ? 1 : 0);
        oscIO->sendOsc(addr + QString::number(8),  led8  ? 1 : 0);
        oscIO->sendOsc(addr + QString::number(9),  led9  ? 1 : 0);
        oscIO->sendOsc(addr + QString::number(10), led10 ? 1 : 0);
        oscIO->sendOsc(addr + QString::number(11), led11 ? 1 : 0);

        oscIO->sendOsc("/POTLed" + QString::number(chan), led12 ? 1 : 0);
    }
}

/**
 * Analyze OSC datas for the vPot scrolling and translate them into Makie control midi datas
 * @brief Translator::vPotScrolling
 * @param chan
 * @param value
 */
void Translator::vPotScrolling(int chan, int value)
{
    int midiVal;
    if(value == 0) midiVal = 65;
    if(value == 1) midiVal = 1;

    midiIO->sendControler(0, 15+chan, midiVal);
}

/**
 * vPot Pressing action
 * @brief Translator::vPotSelectChannel
 * @param p
 * @param chan
 * @param value
 */
void Translator::vPotSelectChannel(Protocol p, int chan, bool value)
{
    btnAction((chan >= 1 && chan <= 8), p, "/POTled" + QString::number(chan), 0, G_1 + chan, value);
}

/**
 * Select btn action
 * @brief Translator::selectChannel
 * @param p
 * @param chan
 * @param value
 */
void Translator::selectChannel(Protocol p, int chan, bool value)
{
    btnAction((chan >= 1 && chan <= 8), p, "/ledSEL" + QString::number(chan), 0, 23+chan, value);
}

/**
 * Mute btn action
 * @brief Translator::muteChannel
 * @param p
 * @param chan
 * @param value
 */
void Translator::muteChannel(Protocol p, int chan, bool value)
{
    btnAction((chan >= 1 && chan <= 8), p, "/ledMUTE" + QString::number(chan), 0, 15+chan, value);
}

/**
 * Solo btn action
 * @brief Translator::soloChannel
 * @param p
 * @param chan
 * @param value
 */
void Translator::soloChannel(Protocol p, int chan, bool value)
{
    btnAction((chan >= 1 && chan <= 8), p, "/ledSOLO" + QString::number(chan), 0, 7+chan, value);
}

/**
 * Rec btn Action
 * @brief Translator::recChannel
 * @param p
 * @param chan
 * @param value
 */
void Translator::recChannel(Protocol p, int chan, bool value)
{
    btnAction((chan >= 1 && chan <= 8), p, "/ledREC" + QString::number(chan), 0, chan-1, value);
}

/**
 * Send fader position throught desired protocol
 * @brief Translator::sendFaderPos
 * @param destProtocol
 * @param fader
 * @param value
 */
void Translator::sendFaderPos(Protocol destProtocol, int fader, int value)
{
    if(isReady() && fader >= 1 && fader <= 9 && value <= 16383)
    {
        if(destProtocol == OSC)
            oscIO->sendOsc("/fader" + QString::number(fader), value);
        else if(destProtocol == MIDI)
            midiIO->sendPitchBend(fader-1, value);
    }
}

/**
 * analyze midi Vu meter datas and send OSC
 * @brief Translator::processVuMeter
 * @param chan
 * @param value
 */
void Translator::processVuMeter(int chan, int value)
{
    bool* tabVuOld = vuMeterStatus[chan];

    for(register int i = 0; i < 12; i++)
    {
        bool status = (value >= i+1);
        if(tabVuOld[i] != status)
        {
            oscIO->sendOsc("/led" + QString::number(chan+1) + QString::number(i+1), status ? 1 : 0);
            tabVuOld[i] = status;
        }
    }
}



//=========================== MASTER / ASSIGNEMENT =============================

/**
 * Assign Track btn action
 * @brief Translator::assignTrack
 * @param p
 * @param value
 */
void Translator::assignTrack(Protocol p, bool value)
{
    btnAction(p, "/ledTrack", 0, E_2, value);
}

/**
 * Assign send btn action
 * @brief Translator::assignSend
 * @param p
 * @param value
 */
void Translator::assignSend(Protocol p, bool value)
{
    btnAction(p, "/ledSend", 0, F_2, value);
}

/**
 * assign pan / surround btn action
 * @brief Translator::assignPanSurround
 * @param p
 * @param value
 */
void Translator::assignPanSurround(Protocol p, bool value)
{
    btnAction(p, "/ledPan", 0, Fd_2, value);
}

/**
 * assign plugin btn action
 * @brief Translator::assignPlugin
 * @param p
 * @param value
 */
void Translator::assignPlugin(Protocol p, bool value)
{
    btnAction(p, "/ledPlugin", 0, G_2, value);
}

/**
 * assign EQ btn action
 * @brief Translator::assignEQ
 * @param p
 * @param value
 */
void Translator::assignEQ(Protocol p, bool value)
{
    btnAction(p, "/ledEQ", 0, Gd_2, value);
}

/**
 * assign instrument btn action
 * @brief Translator::assignInstrument
 * @param p
 * @param value
 */
void Translator::assignInstrument(Protocol p, bool value)
{
    btnAction(p, "/ledInstrument", 0, A_2, value);
}

/**
 * Bank Left btn action
 * @brief Translator::bankLeft
 * @param p
 */
void Translator::bankLeft(Protocol p, bool value)
{
    btnAction(p, "/BankL", 0, Ad_2, value);
}

/**
 * Bank Right btn action
 * @brief Translator::bankRight
 * @param p
 */
void Translator::bankRight(Protocol p, bool value)
{
    btnAction(p, "/BankR", 0, B_2, value);
}

/**
 * Channel left btn action
 * @brief Translator::channelLeft
 * @param p
 */
void Translator::channelLeft(Protocol p, bool value)
{
    btnAction(p, "/ChannelL", 0, C_3, value);
}

/**
 * Channel Right btn action
 * @brief Translator::channelRight
 * @param p
 */
void Translator::channelRight(Protocol p, bool value)
{
    btnAction(p, "/ChannelR", 0, Cd_3, value);
}

/**
 * Flip btn action
 * @brief Translator::flip
 * @param p
 * @param value
 */
void Translator::flip(Protocol p, bool value)
{
    btnAction(p, "/ledFlip", 0, D_3, value);
}

/**
 * Global btn actions
 * @brief Translator::global
 * @param p
 * @param value
 */
void Translator::global(Protocol p, bool value)
{
    btnAction(p, "/ledGlobal", 0, Dd_3, value);
}



//=============================== FUNCTIONS =====================================

/**
 * F_Something btn action
 * @brief Translator::functionBtn
 * @param p
 * @param nbre
 */
void Translator::functionBtn(Protocol p, int nbre, bool value)
{
    btnAction(p, "/F" + QString::number(nbre), 0, F_3 + nbre, value);
}

/**
 * Midi tracks btn action
 * @brief Translator::midiTracks
 * @param p
 */
void Translator::midiTracks(Protocol p, bool value)
{
    btnAction(p, "/MidiTracks", 0, D_4, value);
}

/**
 * Inputs btn action
 * @brief Translator::inputs
 * @param p
 */
void Translator::inputs(Protocol p, bool value)
{
    btnAction(p, "/Inputs", 0, Dd_4, value);
}

/**
 * audio track btn action
 * @brief Translator::audioTracks
 * @param p
 */
void Translator::audioTracks(Protocol p, bool value)
{
    btnAction(p, "/AudioTracks", 0, E_4, value);
}

/**
 * audio instrument btn action
 * @brief Translator::audioInstrument
 * @param p
 */
void Translator::audioInstrument(Protocol p, bool value)
{
    btnAction(p, "/AudioInstrument", 0, F_4, value);
}

/**
 * Aux btn action
 * @brief Translator::aux
 * @param p
 */
void Translator::aux(Protocol p, bool value)
{
    btnAction(p, "/Aux", 0, Fd_4, value);
}

/**
 * Busses btn action
 * @brief Translator::busses
 * @param p
 */
void Translator::busses(Protocol p, bool value)
{
    btnAction(p, "/Busses", 0, G_4, value);
}

/**
 * outputs btn action
 * @brief Translator::outputs
 * @param p
 */
void Translator::outputs(Protocol p, bool value)
{
    btnAction(p, "/Outputs", 0, Gd_4, value);
}

/**
 * user btn action
 * @brief Translator::user
 * @param p
 */
void Translator::user(Protocol p, bool value)
{
    btnAction(p, "/User", 0, A_4, value);
}

/**
 * shift btn action
 * @brief Translator::shift
 * @param p
 */
void Translator::shift(Protocol p, bool value)
{
    btnAction(p, "/Shift", 0, Ad_4, value);
}

/**
 * Option btn action
 * @brief Translator::option
 * @param p
 */
void Translator::option(Protocol p, bool value)
{
    btnAction(p, "/Option", 0, B_4, value);
}

/**
 * control btn action
 * @brief Translator::control
 * @param p
 */
void Translator::control(Protocol p, bool value)
{
    btnAction(p, "/Control", 0, C_5, value);
}

/**
 * alt btn action
 * @brief Translator::alt
 * @param p
 */
void Translator::alt(Protocol p, bool value)
{
    btnAction(p, "/Alt", 0, Cd_5, value);
}

/**
 * read / off btn action
 * @brief Translator::readOff
 * @param p
 * @param value
 */
void Translator::readOff(Protocol p, bool value)
{
    btnAction(p, "/ledReadOff", 0, D_5, value);
}

/**
 * write btn action
 * @brief Translator::write
 * @param p
 * @param value
 */
void Translator::write(Protocol p, bool value)
{
    btnAction(p, "/ledWrite", 0, Dd_5, value);
}

/**
 * trim btn action
 * @brief Translator::trim
 * @param p
 * @param value
 */
void Translator::trim(Protocol p, bool value)
{
    btnAction(p, "/ledTrim", 0, E_5, value);
}

/**
 * touch btn action
 * @brief Translator::touch
 * @param p
 * @param value
 */
void Translator::touch(Protocol p, bool value)
{
    btnAction(p, "/ledTouch", 0, F_5, value);
}

/**
 * latch btn action
 * @brief Translator::latch
 * @param p
 * @param value
 */
void Translator::latch(Protocol p, bool value)
{
    btnAction(p, "/ledLatch", 0, Fd_5, value);
}

/**
 * group btn action
 * @brief Translator::group
 * @param p
 * @param value
 */
void Translator::group(Protocol p, bool value)
{
    btnAction(p, "/ledGroup", 0, G_5, value);
}

/**
 * save btn action
 * @brief Translator::save
 * @param p
 * @param value
 */
void Translator::save(Protocol p, bool value)
{
    btnAction(p, "/ledSave", 0, Gd_5, value);
}

/**
 * undo btn actions
 * @brief Translator::undo
 * @param p
 * @param value
 */
void Translator::undo(Protocol p, bool value)
{
    btnAction(p, "/ledUndo", 0, A_5, value);
}

/**
 * cancel btn action
 * @brief Translator::cancel
 * @param p
 */
void Translator::cancel(Protocol p, bool value)
{
    btnAction(p, "/Cancel", 0, Ad_5, value);
}

/**
 * enter btn action
 * @brief Translator::enter
 * @param p
 */
void Translator::enter(Protocol p, bool value)
{
    btnAction(p, "/Enter", 0, B_5, value);
}

/**
 * marker btn action
 * @brief Translator::markers
 * @param p
 * @param value
 */
void Translator::markers(Protocol p, bool value)
{
    btnAction(p, "/ledMarkers", 0, C_6, value);
}

/**
 * nudge btn action
 * @brief Translator::nudge
 * @param p
 * @param value
 */
void Translator::nudge(Protocol p, bool value)
{
    btnAction(p, "/ledNudge", 0, Cd_6, value);
}

/**
 * cycle btn action
 * @brief Translator::cycle
 * @param p
 * @param value
 */
void Translator::cycle(Protocol p, bool value)
{
    btnAction(p, "/ledCycle", 0, D_6, value);
}

/**
 * drop btn action
 * @brief Translator::drop
 * @param p
 * @param value
 */
void Translator::drop(Protocol p, bool value)
{
    btnAction(p, "/ledDrop", 0, Dd_6, value);
}

/**
 * replace btn action
 * @brief Translator::replace
 * @param p
 * @param value
 */
void Translator::replace(Protocol p, bool value)
{
    btnAction(p, "/ledReplace", 0, E_6, value);
}

/**
 * click btn action
 * @brief Translator::click
 * @param p
 * @param value
 */
void Translator::click(Protocol p, bool value)
{
    btnAction(p, "/ledClick", 0, F_6, value);
}

/**
 * solo btn action
 * @brief Translator::solo
 * @param p
 * @param value
 */
void Translator::solo(Protocol p, bool value)
{
    btnAction(p, "/ledSolo", 0, Fd_6, value);
}



//=============================== TRANSPORT =====================================

/**
 * rewind btn action
 * @brief Translator::rewind
 * @param p
 * @param value
 */
void Translator::rewind(Protocol p, bool value)
{
    btnAction(p, "/ledRewind", 0, G_6, value);
}

/**
 * forward btn action
 * @brief Translator::forward
 * @param p
 * @param value
 */
void Translator::forward(Protocol p, bool value)
{
    btnAction(p, "/ledForward", 0, Gd_6, value);
}

/**
 * stop btn action
 * @brief Translator::stop
 * @param p
 * @param value
 */
void Translator::stop(Protocol p, bool value)
{
    btnAction(p, "/ledStop", 0, A_6, value);
}

/**
 * play btn action
 * @brief Translator::play
 * @param p
 * @param value
 */
void Translator::play(Protocol p, bool value)
{
    btnAction(p, "/ledPlay", 0, Ad_6, value);
}

/**
 * rec btn action
 * @brief Translator::rec
 * @param p
 * @param value
 */
void Translator::rec(Protocol p, bool value)
{
    btnAction(p, "/ledRec", 0, B_6, value);
}

/**
 * up btn action
 * @brief Translator::up
 * @param p
 */
void Translator::up(Protocol p, bool value)
{
    btnAction(p, "/Up", 0, C_7, value);
}

/**
 * down btn action
 * @brief Translator::down
 * @param p
 */
void Translator::down(Protocol p, bool value)
{
    btnAction(p, "/Down", 0, Cd_7, value);
}

/**
 * zoom btn action
 * @brief Translator::zoom
 * @param p
 * @param value
 */
void Translator::zoom(Protocol p, bool value)
{
    btnAction(p, "/ledZoom", 0, Dd_7, value);
}

/**
 * left btn action
 * @brief Translator::left
 * @param p
 */
void Translator::left(Protocol p, bool value)
{
    btnAction(p, "/Left", 0, E_7, value);
}

/**
 * right btn action
 * @brief Translator::right
 * @param p
 */
void Translator::right(Protocol p, bool value)
{
    btnAction(p, "/Right", 0, F_7, value);
}

/**
 * scrb btn action
 * @brief Translator::scrub
 * @param p
 * @param value
 */
void Translator::scrub(Protocol p, bool value)
{
    btnAction(p, "/ledScrub", 0, D_7, value);
}

/**
 * analyze OSC scrub scrolling datas and translate them in midi
 * @brief Translator::scrubScrolling
 * @param value
 */
void Translator::scrubScrolling(int value)
{
    int midiVal;
    if(value == 0) midiVal = 65;
    if(value == 1) midiVal = 1;

    midiIO->sendControler(0, 60, midiVal);
}
