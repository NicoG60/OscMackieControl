/*!
 * \class MidiIO
 * \author Nicolas Jarnoux
 * \date 15/10/2018
 *
 * \brief Representing midi input/output
 *
 * this class is a wrapper from RtMidi lib to Qt system. It is a singleton, you
 * can call slots to send messages and listen to signals to recieve them.
 * iMidi is a macro calling the instance
 */
#ifndef MIDIIO_H
#define MIDIIO_H

#include <QObject>
#include <RtMidi.h>

#include "midiNote.h"

class MidiIO : public QObject
{
	Q_OBJECT

protected:
	/*!
	 * \brief MidiIO constructs the object with default state.
	 * \param parent parent object, will be nullptr anyway.
	 */
	MidiIO(QObject* parent = nullptr);

	/*!
	 * \brief ~MidiIO destruct the object properly
	 */
	~MidiIO();


public:
	static MidiIO* instance();

	/*!
	 * \brief isOpen test if the midi ports are opened or not.
	 * \return True if both are open, otherwise false
	 */
	bool isOpen() { return _midiIn->isPortOpen() && _midiOut->isPortOpen(); }


	/**
	 * @brief getAvailableInputPort returns a list of input ports names
	 * @return A string list with ports name indexed like in RtMidi
	 */
	QStringList getAvailableInputPort();

	/**
	 * @brief getAvailableInputPort returns a list of output ports names
	 * @return A string list with ports name indexed like in RtMidi
	 */
	QStringList getAvailableOutputPort();


	/*!
	 * \brief connectInputPort connect to the given input
	 * \param index : the index in the list of ports
	 * \return True on success, false on failure
	 */
	bool connectInputPort(uint index);

	/*!
	 * \brief This is an overloaded function
	 * \see connectInputPort
	 */
	bool connectInputPort(QString name);


	/*!
	 * \brief connectOutputPort connect to the given output
	 * \param index : the index in the list of ports
	 * \return True on success, false on failure
	 */
	bool connectOutputPort(uint index);

	/*!
	 * \brief This is an overloaded function
	 * \see connectOutputPort
	 */
	bool connectOutputPort(QString name);


	/*!
	 * \brief getStatus returns a string with the current port status.
	 * \return current status ready to display.
	 */
	QString getStatus();


	/*!
	 * \brief getInputPortIndex
	 * \return the current input port
	 */
	uint getInputPortIndex() { return _inPort; }

	/*!
	 * \brief getOutputPortIndex
	 * \return the current output port
	 */
	uint getOutputPortIndex() { return _outPort; }

signals:
	/*!
	 * All these signals are emited when the corresponding midi message is recieved.
	 */
	void midiNoteOff		(const uchar chan, const uchar note,	const uchar vel);
	void midiNoteOn			(const uchar chan, const uchar note,	const uchar vel);
	void midiKeyPressure	(const uchar chan, const uchar note,	const uchar vel);
	void midiControler		(const uchar chan, const uchar control, const uchar value);
	void midiProgram		(const uchar chan, const uchar program);
	void midiChannelPressure(const uchar chan, const uchar value);
	void midiPitchBend		(const uchar chan, const int   value);
	void midiSysex			(const QByteArray &data);
	void midiSystemCommon	(const uchar status);
	void midiSystemRealtime	(const uchar status);

public slots:
	/*!
	 * All these slots are for sending the corresponding midi message.
	 */
	void sendNoteOff		(const uchar chan, const uchar note,	const uchar vel);
	void sendNoteOn			(const uchar chan, const uchar note,	const uchar vel);
	void sendKeyPressure	(const uchar chan, const uchar note,	const uchar vel);
	void sendControler		(const uchar chan, const uchar control, const uchar value);
	void sendProgram		(const uchar chan, const uchar program);
	void sendChannelPressure(const uchar chan, const uchar value);
	void sendPitchBend		(const uchar chan, const int   value);
	void sendSysex			(const QByteArray &data);

private:
	/*!
	 * \brief readMidiInput The callback RtMidi calls when a message is recieved
	 * \param timestamp : timestamp of the message (unused)
	 * \param msg : the message as a vector og uchar
	 * \param data : user defined data passed to the callback. it is "this" in out case
	 */
	static void readMidiInput(double timestamp, std::vector< unsigned char > *msg, void *data);

	//===== HELPER FUNCTIONS
	/*!
	 * These three functions are abstraction of the public corresponding functions
	 */
	QStringList getAvailablePort(RtMidi* port);
	bool connectPort(RtMidi* port, uint index, uint& intPort);
	bool connectPort(RtMidi* port, QString name, uint& intPort);

	/*!
	 * These two function are helpers for sending slots.
	 */
	void send(const uchar type, const uchar chan, const uchar data1);
	void send(const uchar type, const uchar chan, const uchar data1, const uchar data2);

private:
	static MidiIO* _instance;

	RtMidiIn*	_midiIn;
	RtMidiOut*	_midiOut;

	uint		_inPort;
	uint		_outPort;
};

#define iMidi MidiIO::instance()

#endif // MIDIIO_H
