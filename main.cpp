#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "uibackend.h"
#include "networkbackend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<UiBackEnd>("uibackend",1,0,"UiBackEnd");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    UiBackEnd master;
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

    return app.exec();
}
