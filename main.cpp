#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "radiostatus.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    RadioStatus radioStatus;
    engine.rootContext()->setContextProperty("radioStatus", &radioStatus);

    engine.load(QUrl(QStringLiteral("../../Main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
