#include <QApplication>
#include "oscmackiecontrolapp.h"

#define _STR(X) #X
#define STRINGIFY(X) _STR(X)

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    QApplication::setApplicationName("OscMackieControl");
    QApplication::setApplicationVersion(QStringLiteral("v%1 (%2)").arg(STRINGIFY(SOFT_VERSION)).arg(STRINGIFY(GIT_VERSION)));

    //QApplication::setQuitOnLastWindowClosed(false);

    qDebug() << QOsc::displayVersion();
    qDebug() << QMidi::displayVersion();

    OscMackieControlApp _;

    return app.exec();
}
