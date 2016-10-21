#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QSettings>

#include "Translator.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_btnConnectOSC_clicked();
    void on_btnConnectMidi_clicked();
    void on_cb_overlays_activated(const QString &arg1);

private:
    void refreshMIDIPort();
    void connectOSC();
    void connectMIDI();
    void refreshStatus();

private:
    Ui::Widget *    ui;
    QSettings       settings;
    QSettings       overlays;
    OscIO*          oscIO;
    MidiIO*         midiIO;
    Translator*     translator;
};

#endif // WIDGET_H
