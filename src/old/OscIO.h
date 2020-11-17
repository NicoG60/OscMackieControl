/*!
 * \class OscIO
 * \author Nicolas Jarnoux
 * \date 15/10/2018
 *
 * \brief Representing osc input/output
 *
 * this class is for convinience purpose only, based on the libosc. It is a singleton, you
 * can call slots to send messages and listen to signals to recieve them.
 * iOsc is a macro calling the instance
 */
#ifndef OSCIO_H
#define OSCIO_H

#include <QObject>
#include <QUdpSocket>

#include <osc/composer/OscMessageComposer.h>
#include <osc/reader/OscReader.h>
#include <osc/reader/OscMessage.h>

class OscIO : public QObject
{
	Q_OBJECT

protected:
	/*!
	 * \brief OscIO construct the object with default state
	 * \param parent : parent object, always null anyway
	 */
	OscIO(QObject* parent = nullptr);

	/*!
	 * \brief destruct the object properly
	 */
	~OscIO();


public:
	static OscIO* instance();

	/*!
	 * \brief isOpen checks if both sockets are opened or not
	 * \return True if both opened, false otherwise
	 */
	bool isOpen() { return _serverOSC->isOpen() && _clientOSC->isOpen(); }

	/*!
	 * \brief connectOsc connects the sockets properly
	 * \param remoteIP : the IP of remote device
	 * \param inPort : port for incoming data
	 * \param outPort : port for outgoing data
	 * \return true on success
	 */
	bool connectOsc(QHostAddress remoteIP, quint16 inPort, quint16 outPort);

	/*!
	 * \brief getStatus returns the status of the object
	 * \return a string ready to display
	 */
	QString getStatus();

signals:
	/*!
	 * \brief oscMessage, signal emitted when a message is recieved
	 * \param msg : an UNSAFE pointer to the message. has to be destroyed by the /!\ reciever /!\
	 */
	void oscMessage(QByteArray raw);

public slots:
	/*!
	 * These slots are for sending osc messages
	 */
	void sendOsc(QString addr, bool b);
	void sendOsc(QString addr, char c);
	void sendOsc(QString addr, float f);
	void sendOsc(QString addr, int i);
	void sendOsc(QString addr, uchar port, uchar status, uchar data1, uchar data2);
	void sendOsc(QString addr, QString s);

private slots:
	/*!
	 * \brief readOSC read pending datagrams from server socket
	 */
	void readOSC();

private:
	/*!
	 * \brief sendOsc abstraction for all public slots above
	 */
	void sendOsc(QString addr, QVariant v);

private:
	static OscIO*	_instance;

	QUdpSocket*		_serverOSC;
	QUdpSocket*		_clientOSC;

	QHostAddress	_localAddress;
	QHostAddress	_remoteAddress;
	quint16			_localPort;
	quint16			_remotePort;

	OscMessage*		_currentOscMessage;
};

#define iOsc OscIO::instance()

#endif // OSCIO_H
