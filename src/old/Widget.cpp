#include <QDebug>
#include <QMessageBox>
#include <QDir>

#include "Widget.h"
#include "ui_Widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat),
	overlays(QCoreApplication::applicationDirPath() + "/Overlays.ini", QSettings::IniFormat),
	translator(new Translator(this))
{
    ui->setupUi(this);

	if(settings.status() != QSettings::NoError)
		QMessageBox::information(this, "Settings Error", QString::number(settings.status()));
	if(overlays.status() != QSettings::NoError)
		QMessageBox::information(this, "Overlays Error", QString::number(overlays.status()));

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

void Widget::on_cb_overlays_activated(const QString &arg1)
{
    overlays.beginGroup(arg1);

    QStringList keys = overlays.childKeys();

    foreach(QString key, keys)
		iOsc->sendOsc("/label"+key, overlays.value(key, "").toString());

    overlays.endGroup();

	for(int i = 0; i < 112; i++)
		iOsc->sendOsc("/char"+QString::number(i+1), QString(" "));
}

void Widget::refreshMIDIPort()
{
    ui->cbInPort->clear();
    ui->cbOutPort->clear();

	ui->cbInPort->addItems(iMidi->getAvailableInputPort());
	ui->cbOutPort->addItems(iMidi->getAvailableOutputPort());
}

void Widget::connectOSC()
{
	iOsc->connectOsc(QHostAddress(ui->leHost->text()),
					 static_cast<quint16>(ui->sbInPort->value()),
					 static_cast<quint16>(ui->sbOutPort->value()));

    refreshStatus();
}

void Widget::connectMIDI()
{
	iMidi->connectInputPort(static_cast<uint>(ui->cbInPort->currentIndex()));
	iMidi->connectOutputPort(static_cast<uint>(ui->cbOutPort->currentIndex()));

    refreshStatus();
}

void Widget::refreshStatus()
{
    QString status;

	status += iOsc->getStatus();

	status += iMidi->getStatus();

    ui->status->setText(status);
}


