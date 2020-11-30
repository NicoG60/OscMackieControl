#ifndef TOUCHOSCBROADCASTER_H
#define TOUCHOSCBROADCASTER_H

#include <QObject>
#include <QTcpServer>

#include <qmdnsengine/server.h>
#include <qmdnsengine/hostname.h>
#include <qmdnsengine/provider.h>

#include "mapping.h"

class HttpRequest;
class TouchOSCBroadcaster : public QObject
{
    Q_OBJECT

public:
    TouchOSCBroadcaster(Mapping* mapping, QObject* parent = nullptr);

private slots:
    void newConnection();

private:
    void readSocket(QTcpSocket* s, HttpRequest* r);
    void serve(QTcpSocket* s, HttpRequest* r);

private:
    Mapping* _mapping;

    QMdnsEngine::Server   _mdnsServer;
    QMdnsEngine::Hostname _mdnsHostname;
    QMdnsEngine::Provider _mdnsProvider;

    QTcpServer* _httpd;
};

#endif // TOUCHOSCBROADCASTER_H
