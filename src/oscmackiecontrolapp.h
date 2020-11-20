#ifndef OSCMACKIECONTROLAPP_H
#define OSCMACKIECONTROLAPP_H

#include <QObject>
#include <QAction>
#include <QMidiInterface>
#include <QSystemTrayIcon>
#include <QQmlApplicationEngine>

#include "backend.h"
#include "iomonitor.h"

class OscMackieControlApp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantMap oscStatus READ oscStatus NOTIFY oscStatusChanged)
    Q_PROPERTY(QVariantMap midiStatus READ midiStatus NOTIFY midiStatusChanged)

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

    // OSC Monitor
    QVariantMap oscStatus() const;
    QVariantMap midiStatus() const;

public slots:
    void resetCounter();

signals:
    void oscStatusChanged();
    void midiStatusChanged();

public:
    QOscInterface*   osc     = nullptr;
    QMidi*           midi    = nullptr;

    Backend*               backend  = nullptr;
    QQmlApplicationEngine* frontend = nullptr;

    QAction*         midiAction = nullptr;
    QAction*         oscAction  = nullptr;
    QMenu*           menu    = nullptr;

    QTimer*    timerCounter;
    IOMonitor* oscMonitor;
    IOMonitor* midiMonitor;
};

#endif // OSCMACKIECONTROLAPP_H
