#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>
#include "fftvisualizer.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);


    QQmlApplicationEngine engine;


    fftVisualizer testV;

    testV.start();

    engine.rootContext()->setContextProperty("fftV", &testV);


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
