#include "oscmackiecontrolapp.h"
#include "iconmanager.h"

#include <QApplication>
#include <QQuickStyle>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QMenu>

OscMackieControlApp::OscMackieControlApp(QObject *parent) :
    QObject(parent)
{
    setupTray();
    setupCommunication();
    setupUi();
}

OscMackieControlApp::~OscMackieControlApp()
{
}

void OscMackieControlApp::setupTray()
{
    auto iconColor = QApplication::palette().text().color();

    menu = new QMenu;

    midiAction = new QAction(menu);
    midiAction->setIcon(_ima.icon(fa::music, {{"color", iconColor}}));
    midiAction->setText("midi");
    midiAction->setEnabled(false);
    menu->addAction(midiAction);

    oscAction = new QAction(menu);
    oscAction->setIcon(_ima.icon(fa::wifi, {{"color", iconColor}}));
    oscAction->setText("osc");
    oscAction->setEnabled(false);
    menu->addAction(oscAction);

#ifdef Q_OS_MAC
    menu->setAsDockMenu();
#else
    auto tray = new QSystemTrayIcon(this);
    tray->setIcon(_ima.icon(fa::sliders, {{"color", iconColor}}));
    tray->setContextMenu(menu);
    tray->show();
#endif
}

void OscMackieControlApp::setupCommunication()
{
    timerCounter = new QTimer(this);

    osc = new QOscInterface(this);
    oscMonitor = new IOMonitor(timerCounter, this);

    connect(osc, &QOscInterface::messageReceived, oscMonitor, &IOMonitor::countIn);
    connect(osc, &QOscInterface::messageSent, oscMonitor, &IOMonitor::countOut);

    connect(osc, &QOscInterface::remoteAddrChanged, this, &OscMackieControlApp::oscStatusChanged);
    connect(osc, &QOscInterface::localAddrChanged,  this, &OscMackieControlApp::oscStatusChanged);
    connect(osc, &QOscInterface::remotePortChanged, this, &OscMackieControlApp::oscStatusChanged);
    connect(osc, &QOscInterface::localPortChanged,  this, &OscMackieControlApp::oscStatusChanged);

    midi = new QMidi(QMidi::UnspecifiedApi, "OscMackieControl", this);
    midiMonitor = new IOMonitor(timerCounter, this);

    connect(midi, &QMidi::messageReceived, midiMonitor, &IOMonitor::countIn);
    connect(midi, &QMidi::messageSent, midiMonitor, &IOMonitor::countOut);

    backend = new Backend(osc, midi, this);

    timerCounter->setInterval(1000);
    connect(timerCounter, &QTimer::timeout, this, &OscMackieControlApp::resetCounter);
    connect(timerCounter, &QTimer::timeout, this, &OscMackieControlApp::oscStatusChanged);
    connect(timerCounter, &QTimer::timeout, this, &OscMackieControlApp::midiStatusChanged);
    timerCounter->start();

#ifdef Q_OS_MAC
    auto iface = midi->createVirtualInterface("OscMackieControl");
    midi->setInputInterface(iface);
    midi->setOutputInterface(iface);

    midi->open();
#endif
}

void OscMackieControlApp::setupUi()
{
    frontend = new QQmlApplicationEngine(this);

    QQuickStyle::setStyle("Material");

    qmlRegisterSingletonInstance("OscMackieControl.app", 1, 0, "App", this);
    qmlRegisterSingletonInstance("OscMackieControl.osc", 1, 0, "QOsc", osc);
    qmlRegisterSingletonInstance("OscMackieControl.midi", 1, 0, "QMidi", midi);
    qmlRegisterSingletonInstance("OscMackieControl.backend", 1, 0, "Backend", backend);
    qmlRegisterSingletonInstance("OscMackieControl.ima", 1, 0, "IconManager", &_ima);

    frontend->load(QUrl("qrc:/main.qml"));
}

QString OscMackieControlApp::settingsPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/settings.json";
}

bool OscMackieControlApp::hasSettings()
{
    return QFile::exists(settingsPath());
}

void OscMackieControlApp::loadSettings(QString path)
{
    if(path.isEmpty())
        path = settingsPath();

    if(!QFile::exists(path))
        return;

    QFile f(settingsPath());
    if(!f.open(QFile::ReadOnly))
        return;

    auto doc = QJsonDocument::fromJson(f.readAll());

    auto obj = doc.object();

    applySettings(obj);
}

void OscMackieControlApp::applySettings(const QJsonObject& obj)
{
    if(obj.contains("remote_addr"))
        osc->setRemoteAddr(QHostAddress(obj["remote_addr"].toString()));

    if(obj.contains("remote_port"))
        osc->setRemotePort(obj["remote_port"].toInt());

    if(obj.contains("local_port"))
        osc->setRemotePort(obj["local_port"].toInt());

#ifndef Q_OS_MAC
    if(midi->isOpen())
        midi->close();

    if(obj.contains("midi_in"))
    {
        auto name = obj["midi_in"].toString();
        auto ifaces = midi->availableInputInterfaces();

        for(auto& i : ifaces)
        {
            if(i.name() == name)
            {
                midi->setInputInterface(i);
                break;
            }
        }
    }

    if(obj.contains("midi_out"))
    {
        auto name = obj["midi_out"].toString();
        auto ifaces = midi->availableOutputInterfaces();

        for(auto& i : ifaces)
        {
            if(i.name() == name)
            {
                midi->setOutputInterface(i);
                break;
            }
        }
    }
#endif

    if(obj.contains("mapping"))
        backend->loadMapping(obj["mapping"].toObject());
}

QJsonObject OscMackieControlApp::dumpSettings()
{
    return {
        { "remote_addr", osc->remoteAddr().toString() },
        { "remote_port", osc->remotePort() },
        { "local_port", osc->localPort() },

#ifndef Q_OS_MAC
        { "midi_in", midi->inputInterface().name() },
        { "midi_out", midi->outputInterface().name() },
#endif

        { "mapping", backend->dumpMapping() }
    };
}

void OscMackieControlApp::saveSettings(QString path)
{
    if(path.isEmpty())
        path = settingsPath();

    QFile f(settingsPath());
    if(!f.open(QFile::WriteOnly))
        return;

    QJsonDocument doc(dumpSettings());

    f.write(doc.toJson());
}

QVariantMap OscMackieControlApp::oscStatus() const
{
    return {
        { "is_listening", osc->isListening() },
        { "remote_addr",  osc->remoteAddr().toString() },
        { "local_addr",   osc->localAddr().toString() },
        { "remote_port",  osc->remotePort() },
        { "local_port",   osc->localPort() },
        { "last_in",      oscMonitor->lastCountIn() },
        { "last_out",     oscMonitor->lastCountOut() },
        { "avg_in",       oscMonitor->averageIn() },
        { "avg_out",      oscMonitor->averageOut() }
    };
}

QVariantMap OscMackieControlApp::midiStatus() const
{
    return {
        { "is_open",      midi->isOpen() },
        { "input_iface",  midi->inputInterface().name() },
        { "output_iface", midi->outputInterface().name() },
        { "last_in",      midiMonitor->lastCountIn() },
        { "last_out",     midiMonitor->lastCountOut() },
        { "avg_in",       midiMonitor->averageIn() },
        { "avg_out",      midiMonitor->averageOut() }
    };
}

void OscMackieControlApp::resetCounter()
{
    QString midiStatus = QStringLiteral("MIDI In %1 msg/s | Out %2 msg/s").arg(midiMonitor->averageIn()).arg(midiMonitor->averageOut());
    QString oscStatus = QStringLiteral("OSC In %1 msg/s | Out %2 msg/s").arg(oscMonitor->averageIn()).arg(oscMonitor->averageOut());

    midiAction->setText(midiStatus);
    oscAction->setText(oscStatus);
    menu->update();
}
