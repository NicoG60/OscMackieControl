#ifndef OSCMACKIECONTROLAPP_H
#define OSCMACKIECONTROLAPP_H

#include <QObject>
#include <QAction>
#include <QMidiInterface>
#include <QSystemTrayIcon>
#include <QQmlApplicationEngine>
#include <QXmlStreamWriter>

#include "backend.h"
#include "iomonitor.h"
#include "touchoscbroadcaster.h"

class OscMackieControlApp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QOscInterface* osc MEMBER osc CONSTANT FINAL)
    Q_PROPERTY(QMidi* midi MEMBER midi CONSTANT FINAL)
    Q_PROPERTY(IOMonitor* oscMonitor MEMBER oscMonitor CONSTANT FINAL)
    Q_PROPERTY(IOMonitor* midiMonitor MEMBER midiMonitor CONSTANT FINAL)
    Q_PROPERTY(Backend* backend MEMBER backend CONSTANT FINAL)
    Q_PROPERTY(Mapping* mapping MEMBER mapping CONSTANT FINAL)

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
    Q_INVOKABLE void saveSettings(QString path = {});

public slots:
    void resetCounter();

public:
    QOscInterface* osc     = nullptr;
    QMidi*         midi    = nullptr;

    Mapping* mapping = nullptr;

    Backend*               backend  = nullptr;
    QQmlApplicationEngine* frontend = nullptr;

    QAction* midiAction = nullptr;
    QAction* oscAction  = nullptr;
    QMenu*   menu       = nullptr;

    QTimer*    timerCounter = nullptr;
    IOMonitor* oscMonitor   = nullptr;
    IOMonitor* midiMonitor  =nullptr;

    TouchOSCBroadcaster* touchOSCBroadcast;
};

#endif // OSCMACKIECONTROLAPP_H
