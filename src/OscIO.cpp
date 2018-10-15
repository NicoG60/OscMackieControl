#include <QNetworkInterface>

#include "OscIO.h"

OscIO* OscIO::_instance = nullptr;

OscIO::OscIO(QObject *parent) :
	QObject(parent),
	_serverOSC(new QUdpSocket(this)),
	_clientOSC(new QUdpSocket(this)),
	_localAddress(QHostAddress::LocalHost),
	_remoteAddress(QHostAddress::LocalHost),
	_localPort(0),
	_remotePort(0)
{
	connect(_serverOSC, &QUdpSocket::readyRead, this, &OscIO::readOSC);
}

OscIO::~OscIO()
{

}

OscIO* OscIO::instance()
{
	if(!_instance)
		_instance = new OscIO();

	return _instance;
}


bool OscIO::connectOsc(QHostAddress remoteIP, quint16 inPort, quint16 outPort)
{
	if(_serverOSC->isOpen()) _serverOSC->close();
	if(_clientOSC->isOpen()) _clientOSC->close();

	foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
	{
		if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
			_localAddress = address;
	}

	_localPort = inPort;

	_remotePort = outPort;
	_remoteAddress = remoteIP;

	_serverOSC->bind(_localPort);

	_serverOSC->open(QIODevice::ReadOnly);
	_clientOSC->open(QIODevice::WriteOnly);

	return isOpen();
}



QString OscIO::getStatus()
{
	QString status;

	//---- OSC SERVER
	status += "OSC Server:\n";
	status += "Status : ";
	if(_serverOSC->isOpen())
	{
		status += "Opened\n";
		status += "IP:port : " + _localAddress.toString() + ":" + QString::number(_localPort) + "\n";
	}
	else
		status += "Closed\n";

	status += "------------------------\n\n";

	//---- OSC CLIENT
	status += "OSC Client:\n";
	status += "Status : ";
	if(_clientOSC->isOpen())
	{
		status += "Opened\n";
		status += "IP:port : " + _remoteAddress.toString() + ":" + QString::number(_remotePort) + "\n";
	}
	else
		status += "Closed\n";

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
	if(!_clientOSC->isOpen())
		return;

	OscMessageComposer msg(addr);

	msg.pushMidi(static_cast<char>(port),
				 static_cast<char>(status),
				 static_cast<char>(data1),
				 static_cast<char>(data2));

	QByteArray* bytes = msg.getBytes();
	_clientOSC->writeDatagram(*bytes, _remoteAddress, _remotePort);
}

void OscIO::sendOsc(QString addr, QString s)
{
	sendOsc(addr, QVariant(s));
}




void OscIO::readOSC()
{
	if(_serverOSC->hasPendingDatagrams())
	{
		qint64 size = _serverOSC->pendingDatagramSize();
		QByteArray raw(static_cast<int>(size), '\0');
		QHostAddress addr;

		_serverOSC->readDatagram(raw.data(), size, &addr);

		if(addr.toIPv4Address() == _remoteAddress.toIPv4Address())
			emit oscMessage(raw);
	}
}




void OscIO::sendOsc(QString addr, QVariant v)
{
	if(!_clientOSC->isOpen())
		return;

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
	_clientOSC->writeDatagram(*bytes, _remoteAddress, _remotePort);
}
