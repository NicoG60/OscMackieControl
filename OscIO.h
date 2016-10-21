#ifndef OSCIO_H
#define OSCIO_H

#include <QObject>
#include <QUdpSocket>

#include "osc/composer/OscMessageComposer.h"
#include "osc/reader/OscReader.h"
#include "osc/reader/OscMessage.h"

class OscIO : public QObject
{
    Q_OBJECT

public:
    OscIO(QObject* parent = 0);
    ~OscIO();

    bool isOpen() { return serverOSC->isOpen() && clientOSC->isOpen(); }

    bool connectOsc(QHostAddress remoteIP, unsigned int inPort, unsigned int outPort);

    QString getStatus();

signals:
    void oscMessage(OscMessage* msg);

public slots:
    void sendOsc(QString addr, bool b);
    void sendOsc(QString addr, char c);
    void sendOsc(QString addr, float f);
    void sendOsc(QString addr, int i);
    void sendOsc(QString addr, uchar port, uchar status, uchar data1, uchar data2);
    void sendOsc(QString addr, QString s);

private slots:
    void readOSC();

private:
    void sendOsc(QString addr, QVariant v);

private:
    QUdpSocket*     serverOSC;
    QUdpSocket*     clientOSC;

    QHostAddress    localAddress;
    QHostAddress    remoteAddress;
    unsigned int    localPort;
    unsigned int    remotePort;
};

#endif // OSCIO_H
