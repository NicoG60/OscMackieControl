#include <QApplication>
#include <version.h>
#include "oscmackiecontrolapp.h"


int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
    QApplication::setApplicationName("OscMackieControl");
    QApplication::setApplicationVersion(QStringLiteral("v%1 (%2)").arg(PROJECT_VERSION, GIT_VERSION));

    QApplication::setQuitOnLastWindowClosed(false);

    OscMackieControlApp _;

    return app.exec();
}
