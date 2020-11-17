/*!
 * \class Translator
 * \author Nicolas Jarnoux
 * \date 15/10/2018
 *
 * \brief Representing osc input/output
 *
 * This class provide the core of translation between Midi and OSC.
 */
#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QDebug>
#include <QTimer>

#include "MidiIO.h"
#include "OscIO.h"

#define  CHECK(s,c,f)      if(beginWith(s,c)) f
#define ECHECK(s,c,f) else if(beginWith(s,c)) f
#define CHECK_ADDR(c,f)   CHECK(oscAddr,c,f)
#define ECHECK_ADDR(c,f) ECHECK(oscAddr,c,f)

class Translator : public QObject
{
	Q_OBJECT

public:
	enum Protocol {
		OSC,
		MIDI
	};


	/*!
	 * Constructor of the translator object.
	 * \brief Translator::Translator
	 * \param parent QObject parent
	 */
	Translator(QObject* parent = nullptr);

	/*!
	 * \brief Translator::~Translator
	 */
	~Translator();


private slots:
	/*!
	 * \brief Called when a midi Note On is recieved
	 * Route to the right function to call depending on note
	 */
	void midiNoteOn(const uchar chan, const uchar note,	const uchar vel);

	/*!
	 * \brief Called when a midi Control Change is recieved
	 * Route to the right function to call depending on the control id : vPot Leds or timecode display
	 */
	void midiControler(const uchar chan, const uchar control, const uchar value);

	/*!
	 * \brief Called when a midi Channel After Touch is recieved.
	 * Only used for VU Meter.
	 */
	void midiChannelPressure(const uchar chan, const uchar value);

	/*!
	 * \brief Called when a midi Pitch Bend is recieved.
	 * Only used for fader position
	 */
	void midiPitchBend(const uchar chan, const int value);

	/*!
	 * \brief Called when a midi System Exclusive is recieved
	 * Only for LCD display
	 */
	void midiSysex(const QByteArray &data);

	/*!
	 * \brief Called when an osc message is recieved.
	 * Route to the right function to call depending on the address.
	 */
	void readOSC(QByteArray raw);

	/*!
	 * \brief Called when the timer timeout to act as a released fader
	 */
	void resetFaderTouched();

private:
	//================= Displays
	/*!
	 * \brief Send Name / Value message through protocol
	 */
	void nameValue(Protocol p, bool value);

	/*!
	 * \brief Send SMPTE / BEATS message through protocol
	 */
	void smpteBeats(Protocol p, bool value);

	/*!
	 * \brief Process the timecode value and send it via Osc
	 */
	void processTimecode(int chan, int value);


	//================= Channels
	/*!
	 * \brief Analyze the midi data for the vPot Leds and send Osc well formed leds datas.
	 */
	void processVPotLed(int chan, int value);

	/*!
	 * \brief Analyze OSC datas for the vPot scrolling and translate them into Makie control midi datas
	 */
	void vPotScrolling(int chan, int value);

	/*!
	 * \brief Send button actions through desired protocol
	 */
	void vPotSelectChannel	(Protocol p, int chan, bool value);
	void selectChannel		(Protocol p, int chan, bool value);
	void muteChannel		(Protocol p, int chan, bool value);
	void soloChannel		(Protocol p, int chan, bool value);
	void recChannel			(Protocol p, int chan, bool value);


	/*!
	 * \brief Send fader position throught desired protocol
	 */
	void sendFaderPos(Protocol destProtocol, int fader, int value);


	/*!
	 * \brief Analyze midi Vu meter datas and send to OSC
	 */
	void processVuMeter(int chan, int value);


	//================= Master / Assignment
	/*!
	 * \brief Send Assign, Bank, Channel, etc button action through the desired protocol
	 */
	void assignTrack		(Protocol p, bool value);
	void assignSend			(Protocol p, bool value);
	void assignPanSurround	(Protocol p, bool value);
	void assignPlugin		(Protocol p, bool value);
	void assignEQ			(Protocol p, bool value);
	void assignInstrument	(Protocol p, bool value);

	void bankLeft			(Protocol p, bool value);
	void bankRight			(Protocol p, bool value);

	void channelLeft		(Protocol p, bool value);
	void channelRight		(Protocol p, bool value);

	void flip				(Protocol p, bool value);
	void global				(Protocol p, bool value);


	//================= Functions
	/*!
	 * \brief Send F_ button action
	 */
	void functionBtn	(Protocol p, int nbre, bool value);

	/*!
	 * \brief Send globl views button action
	 */
	void midiTracks		(Protocol p, bool value);
	void inputs			(Protocol p, bool value);
	void audioTracks	(Protocol p, bool value);
	void audioInstrument(Protocol p, bool value);
	void aux			(Protocol p, bool value);
	void busses			(Protocol p, bool value);
	void outputs		(Protocol p, bool value);
	void user			(Protocol p, bool value);

	/*!
	 * \brief Send modifier button action
	 */
	void shift			(Protocol p, bool value);
	void option			(Protocol p, bool value);
	void control		(Protocol p, bool value);
	void alt			(Protocol p, bool value);

	/*!
	 * \brief Send automation button action
	 */
	void readOff	(Protocol p, bool value);
	void write		(Protocol p, bool value);
	void trim		(Protocol p, bool value);
	void touch		(Protocol p, bool value);
	void latch		(Protocol p, bool value);
	void group		(Protocol p, bool value);

	/*!
	 * \brief Send utilities button actions
	 */
	void save		(Protocol p, bool value);
	void undo		(Protocol p, bool value);
	void cancel		(Protocol p, bool value);
	void enter		(Protocol p, bool value);

	/*!
	 * \brief Send transport button action
	 */
	void markers	(Protocol p, bool value);
	void nudge		(Protocol p, bool value);
	void cycle		(Protocol p, bool value);
	void drop		(Protocol p, bool value);
	void replace	(Protocol p, bool value);
	void click		(Protocol p, bool value);
	void solo		(Protocol p, bool value);

	void rewind		(Protocol p, bool value);
	void forward	(Protocol p, bool value);
	void stop		(Protocol p, bool value);
	void play		(Protocol p, bool value);
	void rec		(Protocol p, bool value);

	/*!
	 * \brief Send Zoom button action
	 */
	void up			(Protocol p, bool value);
	void down		(Protocol p, bool value);
	void zoom		(Protocol p, bool value);
	void left		(Protocol p, bool value);
	void right		(Protocol p, bool value);

	/*!
	 * \brief Sned job actions
	 */
	void scrub		(Protocol p, bool value);
	void scrubScrolling(int value);

	//======= HELPER FUNCTIONS
private:
	/*!
	 * \brief Check if \a addr begin with \a check
	 * \param addr : string to check
	 * \param check : string it has to begin by
	 * \return true if addr begins by check, false otherwise
	 */
	bool beginWith(QString addr, QString check);

	/*!
	 * \brief Send an instant On/Off cycle on a note.
	 */
	void bangNote(int chan, int note, int vel);

	/*!
	 * \brief General case of an action on a button.
	 * \param condition : condition to send data through protocol
	 * \param destProtocol : the destination protocol where data will be sent
	 * \param addrOSC : the OSC address where to send message
	 * \param midiChan : the midi channel where to send message
	 * \param midiNote : the midi note on action
	 * \param value : true : btn down, false btn up
	 */
	void btnAction(bool condition, Protocol destProtocol, QString addrOSC, int midiChan, int midiNote, bool value);

	/*!
	 * \brief This is an overloaded function where \a condition is always true
	 * \see btnAction
	 */
	void btnAction(Protocol destProtocol, QString addrOSC, int midiChan, int midiNote, bool value);

	/*!
	 * \brief Set a fader to touched state and start a timer to reset it 100ms later
	 * Each time a fader is moved by the OSC, that is, by the user, a touched signal
	 * is send the first.
	 * Each move of the fader make a timer restart. If the timer timeout, that means
	 * that the fader has not been moved for 100ms so, a release signal is sent.
	 */
	void setFaderTouched(int chan);


private:
	QByteArray 	_timecode;

	bool** 		_vuMeterStatus;

	QTimer		_touchedTimer;
	bool		_touchedFaders[9];
};

#endif // TRANSLATOR_H
