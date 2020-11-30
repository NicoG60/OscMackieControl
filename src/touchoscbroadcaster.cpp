#include "touchoscbroadcaster.h"

#include <QTcpSocket>
#include <QUrlQuery>
#include <qmdnsengine/service.h>
#include <qmdnsengine/message.h>
#include <QBuffer>

class HttpRequest
{
    friend class TouchOSCBroadcaster;

public:
    enum Method
    {
        GET,
        PUT,
        DELETE,
        POST,
        HEAD,
        OPTIONS,
        PATCH,
        CONNECT
    };

    enum ParserState
    {
        NotStarted,
        ParsingMethod,
        ParsingUri,
        ParsingVersion,
        ParsingHeaders,
        MessagedParsed
    };

public:
    HttpRequest() = default;

    QByteArray url;
    QByteArray method;
    QHash<QByteArray, QByteArray> headers;
    QHostAddress remoteAddress;

    QByteArray header(const QByteArray& key) const
    {
        return headers[key.toLower()];
    }

private:
    bool parse(QIODevice* dev);
    void clear();

private:
    bool handling = false;
    ParserState state = NotStarted;
    QByteArray tmp;
};


bool HttpRequest::parse(QIODevice* dev)
{
    auto data = tmp + dev->readAll();

    if(state == NotStarted)
    {
        tmp.clear();
        state = ParsingMethod;
    }

    static QRegularExpression exp(R"(HTTP\/1\.[01]\r)" );

    auto it = data.begin();
    while(it != data.end())
    {
        switch(state)
        {
        case ParsingMethod:
            while(*it != ' ')
                tmp.append(*(it++));

            method = tmp;
            tmp.clear();
            state = ParsingUri;
            break;

        case ParsingUri:
            while(*it != ' ')
                tmp.append(*(it++));

            url = tmp;
            tmp.clear();
            state = ParsingVersion;
            break;

        case ParsingVersion:
            while(*it != '\n')
                tmp.append(*(it++));

            if(!exp.match(QString(tmp)).hasMatch())
                return false;
            tmp.clear();
            state = ParsingHeaders;
            break;

        case ParsingHeaders:
            while(*it != '\n')
                tmp.append(*(it++));

            if(tmp.back() != '\r')
                return false;

            if(tmp.size() > 1)
            {
                int i = tmp.indexOf(':');

                if(i <= 0)
                    return false;

                headers.insert(tmp.left(i), tmp.mid(i+1).trimmed());
            }
            else
                state = MessagedParsed;

            tmp.clear();
            break;

        default:
            break;
        }

        ++it;
    }

    return true;
}

void HttpRequest::clear()
{
    url.clear();
    headers.clear();
    remoteAddress.clear();
    handling = false;
    state = NotStarted;
    tmp.clear();
}



// =============================================================================



TouchOSCBroadcaster::TouchOSCBroadcaster(Mapping* mapping, QObject* parent) :
    QObject(parent),
    _mapping(mapping),
    _mdnsHostname(&_mdnsServer),
    _mdnsProvider(&_mdnsServer, &_mdnsHostname),
    _httpd(new QTcpServer(this))
{
    connect(&_mdnsServer, &QMdnsEngine::Server::error, [](const QString& str) { qDebug() << "mDNS error:" << str; });

    QMdnsEngine::Service service;
    service.setType("_touchosceditor._tcp.local.");
    service.setName("OscMackieControl");
    service.setPort(9658);
    _mdnsProvider.update(service);

    connect(_httpd, &QTcpServer::newConnection, this, &TouchOSCBroadcaster::newConnection);
    _httpd->listen(QHostAddress::Any, 9658);
}

void TouchOSCBroadcaster::newConnection()
{
    while(auto s = _httpd->nextPendingConnection())
    {
        auto r = new HttpRequest;
        r->remoteAddress = s->peerAddress();

        connect(s, &QTcpSocket::readyRead,
        [this, s, r]()
        {
            readSocket(s, r);
        });

        connect(s, &QTcpSocket::disconnected,
        [s, r]()
        {
            if(!r->handling)
                s->deleteLater();
        });

        connect(s, &QTcpSocket::destroyed, [r]() { delete r; });
    }
}

void TouchOSCBroadcaster::readSocket(QTcpSocket* s, HttpRequest* r)
{
    if(!s->isTransactionStarted())
        s->startTransaction();

    if (r->state == HttpRequest::MessagedParsed)
        r->clear();

    if (!r->parse(s))
    {
        s->disconnect();
        return;
    }

    if(r->state != HttpRequest::MessagedParsed)
        return; //partial read

    s->commitTransaction();
    r->handling = true;
    serve(s, r);
    r->handling = false;
    if (s->state() == QAbstractSocket::UnconnectedState)
        s->deleteLater();
}

void TouchOSCBroadcaster::serve(QTcpSocket* s, HttpRequest* r)
{
    Q_UNUSED(r);

    QBuffer b;
    b.open(QIODevice::WriteOnly);

    b.write("HTTP/1.0 200 OK \r\n");
    b.write("Content-Type: application/touchosc\r\n");
    b.write("Date: ");
    b.write(QDateTime::currentDateTime().toString("ddd, dd MMM yyyy HH:mm:ss t").toLocal8Bit());
    b.write("\r\n");
    b.write("Content-Disposition: attachment; filename=\"OscMackieControl.touchosc\"\r\n");
    b.write("\r\n");

    _mapping->exportTouchOSCLayout(&b);

    s->write(b.data().chopped(1));

    s->close();
}

