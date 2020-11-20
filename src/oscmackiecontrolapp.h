#ifndef OSCMACKIECONTROLAPP_H
#define OSCMACKIECONTROLAPP_H

#include <QObject>
#include <QAction>
#include <QMidiInterface>
#include <QSystemTrayIcon>
#include <QQmlApplicationEngine>
#include "backend.h"

class OscMackieControlApp : public QObject
{
    Q_OBJECT

public:
    explicit OscMackieControlApp(QObject *parent = nullptr);
    ~OscMackieControlApp();

    void setupTray();
    void setupCommunication();
    void setupUi();

    QString settingsPath();
    bool hasSettings();
    void loadSettings(QString path = {});
    void applySettings(const QJsonObject& obj);
    QJsonObject dumpSettings();
    void saveSettings(QString path = {});

public slots:
    inline void midiIn(const QByteArray&) { midiInCount++; }
    inline void midiOut() { midiOutCount++; }
    inline void oscIn(const QOscMessage&) { oscInCount++; }
    inline void oscOut() { oscOutCount++; }
    void resetCounter();

signals:
    void midiStatusUpdate(const QString& str);
    void oscStatusUpdate(const QString& str);

public:
    QOscInterface*   osc     = nullptr;
    QMidi*           midi    = nullptr;

    Backend*               backend  = nullptr;
    QQmlApplicationEngine* frontend = nullptr;

    QAction*         midiAction = nullptr;
    QAction*         oscAction  = nullptr;
    QMenu*           menu    = nullptr;

    QTimer  timerCounter;
    quint64 midiInCount  = 0;
    quint64 midiOutCount = 0;
    quint64 oscInCount  = 0;
    quint64 oscOutCount = 0;
};

#endif // OSCMACKIECONTROLAPP_H
