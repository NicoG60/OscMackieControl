#include <QGuiApplication>
#include "oscmackiecontrolapp.h"

#define _STR(X) #X
#define STRINGIFY(X) _STR(X)

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    QGuiApplication::setApplicationName("OscMackieControl");
    QGuiApplication::setApplicationVersion(QStringLiteral("v%1 (%2)").arg(STRINGIFY(SOFT_VERSION)).arg(STRINGIFY(GIT_VERSION)));

    qDebug() << QOsc::displayVersion();
    qDebug() << QMidi::displayVersion();

    OscMackieControlApp _;

    return a.exec();
}
