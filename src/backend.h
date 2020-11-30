#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QOsc>
#include <QMidi>
#include <QTimer>
#include <array>

#include "mapping.h"

class Backend : public QObject
{
    Q_OBJECT
public:
    struct TrackState
    {
        QTimer faderTimer;
        bool faderTouched = false;
        std::array<bool, 12> vuMeter = { false, false, false, false, false, false, false, false, false, false, false, false };
    };

public:
    explicit Backend(QOscInterface* osc, QMidi* midi, Mapping* mapping, QObject *parent = nullptr);

    void applyMapping();

private slots: // Midi callbacks
    void midiNoteOn(quint8 chan, quint8 note, quint8 vel);
    void midiControler(quint8 chan, quint8 control, uchar value);
    void midiChannelPressure(quint8 chan, quint8 value);
    void midiPitchBend(quint8 chan, quint16 value);
    void midiSysex(const QByteArray& data);

private slots: // Osc Callbacks
    void oscReceived(const QOscMessage& msg);

private slots: // ====== Helpers
    void bangNote(quint8 chan, quint8 note, quint8 vel);

    void sendButtonAction(quint8 note, bool pressed);
    void sendButtonAction(const QString& addr, bool pressed);

    void setFaderTouched(quint8 track);
    void resetFaderTouched(quint8 trackm1);

    void processScrub(int value);
    void processFaderPos(quint8 track, float pos);
    void processVPotScroll(quint8 track, int value);
    void processVPotLed(quint8 track, quint8 value);
    void processTimecode(quint8 chan, quint8 value);
    void processVuMeter(quint8 track, quint8 value);

    void midiFader(quint8 track, quint16 value);

    void sendLcdText(const QByteArray& data);

    QString craftAddr(const QString& base, int id);

private:
    void applyButtonTrack(const ButtonControl& btn, int i);
    void applyButton(const ButtonControl& btn);
    bool overlaps(int start, int end, int a, int b);

signals:

private:
    QOscInterface* _osc;
    QMidi*         _midi;
    Mapping*       _mapping;

    QHash<quint8, QString> _noteButtonMap;
    QHash<QString, quint8> _oscButtonMap;

    QString _lcd;

    std::array<TrackState, 9> _tracks;
};

#endif // BACKEND_H
