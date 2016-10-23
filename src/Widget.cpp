#include <QDebug>
#include <QMessageBox>
#include <QDir>

#include "Widget.h"
#include "ui_Widget.h"

/**
 * Initialize the window, load settings, etc...
 * @brief Widget::Widget
 * @param parent
 */
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat),
    overlays(QCoreApplication::applicationDirPath() + "/Overlays", QSettings::IniFormat),
    oscIO(new OscIO(this)),
    midiIO(new MidiIO(this)),
    translator(new Translator(oscIO, midiIO, this))
{
    ui->setupUi(this);

    if(settings.status() != QSettings::NoError) QMessageBox::information(this, "Settings Error", QString::number(settings.status()));
    if(overlays.status() != QSettings::NoError) QMessageBox::information(this, "Overlays Error", QString::number(overlays.status()));

    ui->cb_overlays->addItems(overlays.childGroups());

    refreshMIDIPort();
    refreshStatus();

    ui->leHost->setText(settings.value("remoteIp", "").toString());
    ui->sbInPort->setValue(settings.value("localPort", 0).toInt());
    ui->sbOutPort->setValue(settings.value("remotePort", 0).toInt());

    ui->cbInPort->setCurrentText(settings.value("midiInPort", "").toString());
    ui->cbOutPort->setCurrentText(settings.value("midiOutPort", "").toString());

    connectOSC();
    connectMIDI();
}

/**
 * save settings and delete window
 * @brief Widget::~Widget
 */
Widget::~Widget()
{
    settings.setValue("remoteIp", ui->leHost->text());
    settings.setValue("localPort", ui->sbInPort->value());
    settings.setValue("remotePort", ui->sbOutPort->value());

    settings.setValue("midiInPort", ui->cbInPort->currentText());
    settings.setValue("midiOutPort", ui->cbOutPort->currentText());

    delete ui;
}

//Slots
void Widget::on_btnConnectOSC_clicked()
{
    connectOSC();
}

void Widget::on_btnConnectMidi_clicked()
{
    connectMIDI();
}

/**
 * Load the overlays depending of the DAW
 * @brief Widget::on_cb_overlays_activated
 * @param arg1
 */
void Widget::on_cb_overlays_activated(const QString &arg1)
{
    overlays.beginGroup(arg1);

    QStringList keys = overlays.childKeys();

    foreach(QString key, keys)
        oscIO->sendOsc("/label"+key, overlays.value(key, "").toString());

    overlays.endGroup();

    for(register int i = 0; i < 112; i++)
        oscIO->sendOsc("/char"+QString::number(i+1), QString(" "));
}

/**
 * Refresh midi combo boxes
 * @brief Widget::refreshMIDIPort
 */
void Widget::refreshMIDIPort()
{
    ui->cbInPort->clear();
    ui->cbOutPort->clear();

    ui->cbInPort->addItems(midiIO->getAvailableInputPort());
    ui->cbOutPort->addItems(midiIO->getAvailableOutputPort());
}

/**
 * connect the osc IO
 * @brief Widget::connectOSC
 */
void Widget::connectOSC()
{
    oscIO->connectOsc(QHostAddress(ui->leHost->text()), ui->sbInPort->value(), ui->sbOutPort->value());

    refreshStatus();
}

/**
 * connect the midi IO
 * @brief Widget::connectMIDI
 */
void Widget::connectMIDI()
{
    midiIO->connectInputPort(ui->cbInPort->currentIndex());
    midiIO->connectOutputPort(ui->cbOutPort->currentIndex());

    refreshStatus();
}

/**
 * display the status of the IO
 * @brief Widget::refreshStatus
 */
void Widget::refreshStatus()
{
    QString status;

    status += oscIO->getStatus();

    status += midiIO->getStatus();

    ui->status->setText(status);
}


