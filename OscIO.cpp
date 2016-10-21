#include <QNetworkInterface>

#include "OscIO.h"

OscIO::OscIO(QObject *parent) :
    QObject(parent),
    serverOSC(new QUdpSocket(this)),
    clientOSC(new QUdpSocket(this)),
    localAddress(QHostAddress::LocalHost),
    remoteAddress(QHostAddress::LocalHost),
    localPort(0),
    remotePort(0)
{
    connect(serverOSC, SIGNAL(readyRead()), this, SLOT(readOSC()));
}

OscIO::~OscIO()
{

}

bool OscIO::connectOsc(QHostAddress remoteIP, unsigned int inPort, unsigned int outPort)
{
    if(serverOSC->isOpen()) serverOSC->close();
    if(clientOSC->isOpen()) clientOSC->close();

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
             localAddress = address;
    }

    localPort = inPort;

    remotePort = outPort;
    remoteAddress = remoteIP;

    serverOSC->bind(localPort);

    serverOSC->open(QIODevice::ReadOnly);
    clientOSC->open(QIODevice::WriteOnly);

    return isOpen();
}

QString OscIO::getStatus()
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

    return status;
}

void OscIO::sendOsc(QString addr, bool b)
{
    sendOsc(addr, QVariant(b));
}

void OscIO::sendOsc(QString addr, char c)
{
    sendOsc(addr, QVariant(c));
}

void OscIO::sendOsc(QString addr, float f)
{
    sendOsc(addr, QVariant(f));
}

void OscIO::sendOsc(QString addr, int i)
{
    sendOsc(addr, QVariant(i));
}

void OscIO::sendOsc(QString addr, uchar port, uchar status, uchar data1, uchar data2)
{
    OscMessageComposer msg(addr);

    msg.pushMidi(port, status, data1, data2);

    QByteArray* bytes = msg.getBytes();
    clientOSC->writeDatagram(*bytes, remoteAddress, remotePort);
}

void OscIO::sendOsc(QString addr, QString s)
{
    sendOsc(addr, QVariant(s));
}

void OscIO::sendOsc(QString addr, QVariant v)
{
    OscMessageComposer msg(addr);

    switch(v.userType())
    {
    case QMetaType::Bool:
        msg.pushBool(v.toBool());
        break;

    case QMetaType::Char:
        msg.pushChar(v.toChar().toLatin1());
        break;

    case QMetaType::Float:
        msg.pushFloat(v.toFloat());
        break;

    case QMetaType::Int:
        msg.pushInt32(v.toInt());
        break;

    case QMetaType::QString:
        msg.pushString(v.toString());
        break;
    }

    QByteArray* bytes = msg.getBytes();
    clientOSC->writeDatagram(*bytes, remoteAddress, remotePort);
}

void OscIO::readOSC()
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

            emit oscMessage(msg);
        }
    }
}
