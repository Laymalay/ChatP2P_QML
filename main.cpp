#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QQuickStyle>
#include "uibackend.h"
#include "networkbackend.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);;
    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("Material");
    UiBackEnd master;
    engine.rootContext()->setContextProperty("master", &master);
    qmlRegisterType<User>();
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    /////////////////////////////////////////////////////////////////////////////////////////////////
    ///                CONNECT WORKER AND MASTER                                                  ///
    /////////////////////////////////////////////////////////////////////////////////////////////////
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    QThread* thread = new QThread;
    NetworkBackEnd* worker = new NetworkBackEnd();
    QObject::connect(thread, &QThread::started, worker, &NetworkBackEnd::process);
    QObject::connect(worker, &NetworkBackEnd::finished, thread, &QThread::quit);
    QObject::connect(worker, &NetworkBackEnd::finished, worker, &NetworkBackEnd::deleteLater);
    QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    worker->moveToThread(thread);
    thread->moveToThread(thread);
    thread->start();
    QObject::connect(&master,&UiBackEnd::signalStartServer,worker, &NetworkBackEnd::slotStartServer);
    QObject::connect(worker,&NetworkBackEnd::sendInfoMessage,&master, &UiBackEnd::slotGetInfoMessage);
    QObject::connect(&app,&QGuiApplication::lastWindowClosed,&master,&UiBackEnd::destructor);
    return app.exec();
}
