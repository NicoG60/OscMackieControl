#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QSettings>

#include "Translator.h"
#include "MidiIO.h"
#include "OscIO.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
	/*!
	 * \brief Initialize the window, load settings, etc...
	 */
	explicit Widget(QWidget *parent = nullptr);

	/*!
	 * \brief save settings and delete window
	 */
    ~Widget();

private slots:
    void on_btnConnectOSC_clicked();
    void on_btnConnectMidi_clicked();

	/*!
	 * \brief Load the overlays depending of the DAW
	 */
    void on_cb_overlays_activated(const QString &arg1);

private:
	/*!
	 * \brief Refresh midi combo boxes
	 */
    void refreshMIDIPort();

	/*!
	 * \brief connect the osc IO
	 */
    void connectOSC();

	/*!
	 * \brief connect the midi IO
	 */
    void connectMIDI();

	/*!
	 * \brief display the status of the IO
	 */
    void refreshStatus();

private:
    Ui::Widget *    ui;
    QSettings       settings;
	QSettings       overlays;
    Translator*     translator;
};

#endif // WIDGET_H
