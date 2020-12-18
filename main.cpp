#include "campaign.h"

#include "viewcontroller.h"

#include <QStringListModel>
#include <QGuiApplication>
#include <QQmlApplicationEngine>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setApplicationName("Game Master's Notepad");
    app.setApplicationVersion("0.1.1");

    qmlRegisterType<ViewController>("org.gmn.viewcontroller", 1, 0, "ViewController");
    qmlRegisterAnonymousType<QStringListModel>("org.gmn", 1);
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
