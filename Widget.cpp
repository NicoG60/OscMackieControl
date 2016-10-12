#include <QDebug>
#include <QNetworkInterface>

#include <osc/reader/types/OscMidi.h>

#include "Widget.h"
#include "ui_Widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    settings("settings.ini", QSettings::IniFormat),
    serverOSC(new QUdpSocket(this)),
    clientOSC(new QUdpSocket(this)),
    midiIO(new MidiIO(this))
{
    ui->setupUi(this);

    connect(serverOSC, SIGNAL(readyRead()), this, SLOT(readOSC()));

    connect(midiIO, SIGNAL(midiNoteOn(int,int,int)), this, SLOT(midiNoteOn(int,int,int)));
    connect(midiIO, SIGNAL(midiNoteOff(int,int,int)), this, SLOT(midiNoteOff(int,int,int)));
    connect(midiIO, SIGNAL(midiKeyPressure(int,int,int)), this, SLOT(midiKeyPressure(int,int,int)));
    connect(midiIO, SIGNAL(midiChannelPressure(int,int)), this, SLOT(midiChannelPressure(int,int)));
    connect(midiIO, SIGNAL(midiPitchBend(int,int)), this, SLOT(midiPitchBend(int,int)));
    connect(midiIO, SIGNAL(midiControler(int,int,int)), this, SLOT(midiControler(int,int,int)));
    connect(midiIO, SIGNAL(midiProgram(int,int)), this, SLOT(midiProgram(int,int)));
    connect(midiIO, SIGNAL(midiSysex(QByteArray)), this, SLOT(midiSysex(QByteArray)));

    refreshMIDIPort();
    refreshStatus();

    ui->leHost->setText(settings.value("remoteIp", "").toString());
    ui->sbInPort->setValue(settings.value("localPort", 0).toInt());
    ui->sbOutPort->setValue(settings.value("remotePort", 0).toInt());

    ui->cbInPort->setCurrentText(settings.value("midiInPort", "").toString());
    ui->cbOutPort->setCurrentText(settings.value("midiOutPort", "").toString());

    connectOSC();
    connectMIDI();
}

Widget::~Widget()
{
    settings.setValue("remoteIp", ui->leHost->text());
    settings.setValue("localPort", ui->sbInPort->value());
    settings.setValue("remotePort", ui->sbOutPort->value());

    settings.setValue("midiInPort", ui->cbInPort->currentText());
    settings.setValue("midiOutPort", ui->cbOutPort->currentText());

    delete ui;
}

bool Widget::isReady()
{
    return serverOSC->isOpen() && clientOSC->isOpen() && midiIO->isOpen();
}

void Widget::refreshMIDIPort()
{
    ui->cbInPort->clear();
    ui->cbOutPort->clear();

    ui->cbInPort->addItems(midiIO->getAvailableInputPort());
    ui->cbOutPort->addItems(midiIO->getAvailableOutputPort());
}

void Widget::connectOSC()
{
    if(serverOSC->isOpen()) serverOSC->close();
    if(clientOSC->isOpen()) clientOSC->close();

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
             localAddress = address;
    }

    localPort = ui->sbInPort->value();

    remotePort = ui->sbOutPort->value();
    remoteAddress = QHostAddress(ui->leHost->text());

    serverOSC->bind(localPort);

    serverOSC->open(QIODevice::ReadOnly);
    clientOSC->open(QIODevice::WriteOnly);

    refreshStatus();
}

void Widget::connectMIDI()
{
    midiIO->connectInputPort(ui->cbInPort->currentIndex());
    midiIO->connectOutputPort(ui->cbOutPort->currentIndex());

    refreshStatus();
}

void Widget::refreshStatus()
{
    QString status;

    //---- OSC SERVER
    status += "Serveur OSC :\n";
    status += "Status : ";
    if(serverOSC->isOpen())
    {
        status += "Ouvert\n";
        status += "IP:port : " + localAddress.toString() + ":" + QString::number(localPort) + "\n";
    }
    else
        status += "Fermé\n";

    status += "------------------------\n\n";

    //---- OSC CLIENT
    status += "Client OSC :\n";
    status += "Status : ";
    if(clientOSC->isOpen())
    {
        status += "Ouvert\n";
        status += "IP:port : " + remoteAddress.toString() + ":" + QString::number(remotePort) + "\n";
    }
    else
        status += "Fermé\n";

    status += "------------------------\n\n";

    status += midiIO->getStatus();

    ui->status->setText(status);
}

void Widget::bangNote(int chan, int note, int vel)
{
    midiIO->sendNoteOn(chan, note, vel);
    midiIO->sendNoteOff(chan, note, 0);
}

void Widget::sendOsc(OscMessageComposer* msg)
{
    QByteArray* bytes = msg->getBytes();
    clientOSC->writeDatagram(*bytes, remoteAddress, remotePort);
}

void Widget::sendOsc(QString addr, float value)
{
    OscMessageComposer msg(addr);

    msg.pushFloat(value);

    sendOsc(&msg);
}

void Widget::sendOsc(QString addr, QString value)
{
    OscMessageComposer msg(addr);

    msg.pushString(value);

    sendOsc(&msg);
}

void Widget::btnAction(bool condition, Protocol destProtocol, QString addrOSC, int midiChan, int midiNote, bool value)
{
    if(isReady() && condition)
    {
        if(destProtocol == OSC)
            sendOsc(addrOSC, value?1:0);
        else if(destProtocol == MIDI)
            bangNote(midiChan, midiNote, value ? 127 : 0);
    }
}

void Widget::sendFaderPos(Protocol destProtocol, int fader, int value)
{
    if(isReady() && fader >= 1 && fader <= 9 && value <= 16383)
    {
        if(destProtocol == OSC)
            sendOsc("/fader" + QString::number(fader), value);
        else if(destProtocol == MIDI)
            midiIO->sendPitchBend(fader-1, value);
    }
}

void Widget::processVPotLed(int chan, int value)
{
    QString addr = "/POTRing" + QString::number(chan);
    if(chan >= 1 && chan <= 8)
    {
        if((value >= 17 && value <=27) || value == 86)
        {
            bool led1 = value == 17;
            bool led2 = value == 17 || value == 18;
            bool led3 = value >= 17 && value <= 19;
            bool led4 = value >= 17 && value <= 20;
            bool led5 = value >= 17 && value <= 21;
            bool led6 = true;
            bool led7 = value >= 23 && value <= 27;
            bool led8 = value >= 24 && value <= 27;
            bool led9 = value >= 25 && value <= 27;
            bool led10 = value == 26 || value == 27;
            bool led11 = value == 27;

            sendOsc(addr + QString::number(1), led1 ? 1 : 0);
            sendOsc(addr + QString::number(2), led2 ? 1 : 0);
            sendOsc(addr + QString::number(3), led3 ? 1 : 0);
            sendOsc(addr + QString::number(4), led4 ? 1 : 0);
            sendOsc(addr + QString::number(5), led5 ? 1 : 0);
            sendOsc(addr + QString::number(6), led6 ? 1 : 0);
            sendOsc(addr + QString::number(7), led7 ? 1 : 0);
            sendOsc(addr + QString::number(8), led8 ? 1 : 0);
            sendOsc(addr + QString::number(9), led9 ? 1 : 0);
            sendOsc(addr + QString::number(10), led10 ? 1 : 0);
            sendOsc(addr + QString::number(11), led11 ? 1 : 0);
        }
        else if(value == 0 || value == 16 || value == 32 || value == 48)
        {
            sendOsc(addr + QString::number(1), false);
            sendOsc(addr + QString::number(2), false);
            sendOsc(addr + QString::number(3), false);
            sendOsc(addr + QString::number(4), false);
            sendOsc(addr + QString::number(5), false);
            sendOsc(addr + QString::number(6), false);
            sendOsc(addr + QString::number(7), false);
            sendOsc(addr + QString::number(8), false);
            sendOsc(addr + QString::number(9), false);
            sendOsc(addr + QString::number(10), false);
            sendOsc(addr + QString::number(11), false);
        }
    }
}

void Widget::on_btnConnectOSC_clicked()
{
    connectOSC();
}

void Widget::on_btnConnectMidi_clicked()
{
    connectMIDI();
}

void Widget::midiNoteOff(const int chan, const int note, const int vel)
{
    Q_UNUSED(chan)
    Q_UNUSED(note)
    Q_UNUSED(vel)
    //qDebug() << "Note Off - Channel : " << chan << ", Note : " << note << ", Vel : " << vel;
}

void Widget::midiNoteOn(const int chan, const int note, const int vel)
{
    qDebug() << "Note On - Channel : " << chan << ", Note : " << note << ", Vel : " << vel;

    if(chan == 0)
    {
        switch(note)
        {
        //0 - 11
        case Cm1: //C-1  : Rec Ch 1
        case Cdm1: //C#-1 : Rec Ch 2
        case Dm1: //D-1  : Rec Ch 3
        case Ddm1: //D#-1 : Rec Ch 4
        case Em1: //E-1  : Rec Ch 5
        case Fm1: //F-1  : Rec Ch 6
        case Fdm1: //F#-1 : Rec Ch 7
        case Gm1: //G-1  : Rec Ch 8
            recChannel(OSC, note+1, vel==127);
            break;

        //12 - 19
        case Gdm1:  //G#-1 : Solo Ch 1
        case Am1:  //A-1  : Solo Ch 2
        case Adm1: //A#-1 : Solo Ch 3
        case Bm1: //B-1  : Solo Ch 4
        case C_0: //C0   : Solo Ch 5
        case Cd_0: //C#0  : Solo Ch 6
        case D_0: //D0   : Solo Ch 7
        case Dd_0: //D#0  : Solo Ch 8
            soloChannel(OSC, note-7, vel==127);
            break;

        //20 - 7
        case E_0: //E0  : Mute Ch 1
        case F_0: //F0  : Mute Ch 2
        case Fd_0: //F#0 : Mute Ch 3
        case G_0: //G0  : Mute Ch 4
        case Gd_0: //G#0 : Mute Ch 5
        case A_0: //A0  : Mute Ch 6
        case Ad_0: //A#0 : Mute Ch 7
        case B_0: //B0  : Mute Ch 8
            muteChannel(OSC, note-15, vel==127);
            break;

        case C_1: // C1  : Select Chn 1
        case Cd_1: // C#1 : Select Chn 2
        case D_1: // D1  : Select Chn 3
        case Dd_1: // D#1 : Select Chn 4
        case E_1: // E1  : Select Chn 5
        case F_1: // F1  : Select Chn 6
        case Fd_1: // F#1 : Select Chn 7
        case G_1: // G1  : Select Chn 8
            selectChannel(OSC, note-23, vel == 127);
            break;

        case Gd_1: //G#1 VPOT Select... Je sais aps ce que c'est!
        case A_1:
        case Ad_1:
        case B_1:
        case C_2:
        case Cd_2:
        case D_2:
        case Dd_2:
            vPotSelectChannel(OSC, note-G_1, vel==127);
            break;

        case E_2: // E2 : Assign Track
            assignTrack(OSC, vel==127);
            break;

        case F_2: // F2 : Assigne Send
            assignSend(OSC, vel==127);
            break;

        case Fd_2: // F#2 : Assign Pan/Surround
            assignPanSurround(OSC, vel==127);
            break;

        case G_2: // G2 : Assign Plug In
            assignPlugin(OSC, vel==127);
            break;

        case Gd_2: // G#2 : Assign EQ
            assignEQ(OSC, vel==127);
            break;

        case A_2: // A2 : Assign Instrument
            assignInstrument(OSC, vel==127);
            break;

        case Ad_2: // A#2 :  Bank Left
            bankLeft(OSC);
            break;

        case B_2: // B2 : Bank Right
            bankRight(OSC);
            break;

        case C_3: // C3 : Channel Left
            channelLeft(OSC);
            break;

        case Cd_3: // C#3 :  Channel Right
            channelRight(OSC);
            break;

        case D_3: // D3 : Flip
            flip(OSC, vel==127);
            break;

        case Dd_3: // D#3 : Edit
            break;

        case E_3: // E3 : Display Name/Value
            break;

        case F_3: // F3 : Display SMPTE/Beats
            break;

        case Fd_3: // F#3 : F1
        case G_3: // G3  : F2
        case Gd_3: // G#3 : F3
        case A_3: // A3  : F4
        case Ad_3: // A#3 : F5
        case B_3: // B3  : F6
        case C_4: // C4  : F7
        case Cd_4: // C#4 : F8
            break;

        case D_4: // D4 : Fader Groups
        case Dd_4:
        case E_4:
        case F_4:
        case Fd_4:
        case G_4:
        case Gd_4:
        case A_4:
            break;

        case Ad_4: // A#4 : Undo?
            break;
        }
    }
}

void Widget::midiKeyPressure(const int chan, const int note, const int vel)
{
    qDebug() << "After Touch - Channel : " << chan << ", Note : " << note << ", Vel : " << vel;
}

void Widget::midiControler(const int chan, const int control, const int value)
{
    qDebug() << "Control Change - Channel : " << chan << ", CC : " << control << ", Value : " << value;
    if(control >= 48 && control <= 55)
        processVPotLed(control-47, value);
}

void Widget::midiProgram(const int chan, const int program)
{
    qDebug() << "Program Change - Channel : " << chan << ", Program : " << program;
}

void Widget::midiChannelPressure(const int chan, const int value)
{
    qDebug() << "Channel Pressure - Channel : " << chan << ", Value : " << value;
}

void Widget::midiPitchBend(const int chan, const int value)
{
    qDebug() << "Pitch Bend - Channel : " << chan << ", Value : " << value;

    if(chan >= 0 && chan <= 8) sendFaderPos(OSC, chan+1, value);
}

void Widget::midiSysex(const QByteArray &data)
{
    QByteArray header;
    header.append((char)0xF0);
    header.append((char)0x0);
    header.append((char)0x0);
    header.append((char)0x66);
    header.append((char)0x14);

    if(data.left(5) == header)
    {
        if(data.at(5) == (char)0x12)
        {
            int start = data.at(6)+1;
            int i = 0;

            while(data.at(7 + i) != (char)0xF7)
            {
                sendOsc("/char" + QString::number(start+i), QString(data.at(7+i)));
                i++;
            }
        }

    }
    qDebug() << "SYSEX : ";

    for(register int i = 0; i < data.size(); i++)
        qDebug() << (uchar)data.at(i);

    qDebug() << "=======";
}

void Widget::midiSystemCommon(const int status)
{
    qDebug() << "System Common : " << status;
}

void Widget::midiSystemRealtime(const int status)
{
    qDebug() << "System Realtime : " << status;
}

void Widget::readOSC()
{
    if(serverOSC->hasPendingDatagrams())
    {
        int size = serverOSC->pendingDatagramSize();
        char* raw = new char[size];
        QHostAddress addr;

        serverOSC->readDatagram(raw, size, &addr);

        QByteArray data(raw, size);



        if(addr.toIPv4Address() == remoteAddress.toIPv4Address())
        {
            OscReader reader(&data);

            OscMessage* msg = reader.getMessage();

            QString oscAddr = msg->getAddress();


            if(oscAddr.left(4) == "/SEL")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    selectChannel(MIDI, oscAddr.right(1).toInt(), true);
            }
            else if(oscAddr.left(5) == "/MUTE")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    muteChannel(MIDI, oscAddr.right(1).toInt(), true);
            }
            else if(oscAddr.left(5) == "/SOLO")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    soloChannel(MIDI, oscAddr.right(1).toInt(), true);
            }
            else if(oscAddr.left(4) == "/REC")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    recChannel(MIDI, oscAddr.right(1).toInt(), true);
            }
            else if(oscAddr == "/BankL")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    bankLeft(MIDI);
            }
            else if(oscAddr == "/BankR")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    bankRight(MIDI);
            }
            else if(oscAddr == "/ChannelL")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    channelLeft(MIDI);
            }
            else if(oscAddr == "/ChannelR")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    channelRight(MIDI);
            }
            else if(oscAddr.left(6) == "/fader")
            {
                float val = msg->getValue(0)->toFloat();
                sendFaderPos(MIDI, oscAddr.right(1).toInt(), static_cast<int>(val));
            }
            else if(oscAddr == "/Track")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    assignTrack(MIDI, true);
            }
            else if(oscAddr == "/Send")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    assignSend(MIDI, true);
            }
            else if(oscAddr == "/Pan")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    assignPanSurround(MIDI, true);
            }
            else if(oscAddr == "/Plugin")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    assignPlugin(MIDI, true);
            }
            else if(oscAddr == "/EQ")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    assignEQ(MIDI, true);
            }
            else if(oscAddr == "/Instrument")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    assignInstrument(MIDI, true);
            }
            else if(oscAddr.left(11) == "/VPotSelect")
            {
                float val = msg->getValue(0)->toFloat();
                if(val == 1)
                    vPotSelectChannel(MIDI, oscAddr.right(1).toInt(), true);
            }
        }
    }
}


void Widget::selectChannel(Protocol p, int chan, bool value)
{
    btnAction((chan >= 1 && chan <= 8), p, "/ledSEL" + QString::number(chan), 0, 23+chan, value);
    qDebug() << "    Channel Select " << chan << " : " << value;
}

void Widget::muteChannel(Protocol p, int chan, bool value)
{
    btnAction((chan >= 1 && chan <= 8), p, "/ledMUTE" + QString::number(chan), 0, 15+chan, value);
    qDebug() << "    Channel Mute " << chan << " : " << value;
}

void Widget::soloChannel(Protocol p, int chan, bool value)
{
    btnAction((chan >= 1 && chan <= 8), p, "/ledSOLO" + QString::number(chan), 0, 7+chan, value);
    qDebug() << "    Channel Solo " << chan << " : " << value;
}

void Widget::recChannel(Protocol p, int chan, bool value)
{
    btnAction((chan >= 1 && chan <= 8), p, "/ledREC" + QString::number(chan), 0, chan-1, value);
    qDebug() << "    Channel Rec " << chan << " : " << value;
}

void Widget::vPotSelectChannel(Protocol p, int chan, bool value)
{
    btnAction((chan >= 1 && chan <= 8), p, "/POTled" + QString::number(chan), 0, G_1 + chan, value);
    qDebug() << "    Channel VPot Select " << chan << " : " << value;
}

void Widget::assignTrack(Protocol p, bool value)
{
    btnAction(p, "/ledTrack", 0, E_2, value);
    qDebug() << "    Track btn : " << value;
}

void Widget::assignSend(Protocol p, bool value)
{
    btnAction(p, "/ledSend", 0, F_2, value);
    qDebug() << "    Send btn : " << value;
}

void Widget::assignPanSurround(Protocol p, bool value)
{
    btnAction(p, "/ledPan", 0, Fd_2, value);
    qDebug() << "    Pan btn : " << value;
}

void Widget::assignPlugin(Protocol p, bool value)
{
    btnAction(p, "/ledPlugin", 0, G_2, value);
    qDebug() << "    Plug-in btn : " << value;
}

void Widget::assignEQ(Protocol p, bool value)
{
    btnAction(p, "/ledEQ", 0, Gd_2, value);
    qDebug() << "    EQ btn : " << value;
}

void Widget::assignInstrument(Protocol p, bool value)
{
    btnAction(p, "/ledInstrument", 0, A_2, value);
    qDebug() << "    Instrument btn : " << value;
}

void Widget::bankLeft(Protocol p)
{
    btnAction(p, "/BankL", 0, Ad_2, true);
    qDebug() << "    Bank Left";
}

void Widget::bankRight(Protocol p)
{
    btnAction(p, "/BankR", 0, B_2, true);
    qDebug() << "    Bank Right";
}

void Widget::channelLeft(Protocol p)
{
    btnAction(p, "/ChannelL", 0, C_3, true);
    qDebug() << "    Channel Left";
}

void Widget::channelRight(Protocol p)
{
    btnAction(p, "/ChannelR", 0, Cd_3, true);
    qDebug() << "    Channel Right Left";
}

void Widget::flip(Protocol p, bool value)
{
    btnAction(p, "/ledFlip", 0, D_3, value);
    qDebug() << "    Flip";
}

void Widget::global(Protocol p, bool value)
{
    Q_UNUSED(p)
    Q_UNUSED(value)
}
