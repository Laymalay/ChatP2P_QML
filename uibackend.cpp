#include "uibackend.h"
#include "networkbackend.h"
UiBackEnd::UiBackEnd(QObject *parent) : QObject(parent)
{
    _comboList = new QStringList();
    _messages = new QStringList();
    _listOfPorts = new QStringList();
    connectToDB();
    getPortsFromDB();
}
QStringList UiBackEnd::comboList()
{
    return *_comboList;
}
QStringList UiBackEnd::messages()
{
    return *_messages;
}


void UiBackEnd::slotGetInfoMessage(QString txt)
{
    _messages->append(txt);
    emit messagesChanged(messages());
}

void UiBackEnd::slotStartServer(QString port)
{
    updateDB(port);
    thisPort = port;
    qDebug()<<"START SERVER WITH PORT: "<<port;
    emit signalStartServer(_listOfPorts,thisPort);
}

void UiBackEnd::destructor()
{
    qDebug()<<"clean"<<thisPort;
    QSqlQuery query(db);
    query.exec("UPDATE ports SET status = 'free' WHERE number =" + thisPort);
}

void UiBackEnd::connectToDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/period-dev/ChatP2P_QML/ports.db");

    if (!db.open())
       {
          qDebug()<<"Cannot open DB:"<<db.lastError();
       }
       else
       {
          qDebug() << "Database: connection ok";
    }
    QSqlQuery query(db);
    query.exec("SELECT number FROM ports");
    while (query.next()) {
       QString port = query.value(0).toString();
       _listOfPorts->append(port);
    }
}

void UiBackEnd::getPortsFromDB()
{
    QSqlQuery query(db);
    query.exec("SELECT number FROM ports WHERE status = 'free'");
    //query.exec("SELECT number FROM ports");
    while (query.next()) {
       QString port = query.value(0).toString();
       _comboList->append(port);
    }
}

void UiBackEnd::updateDB(QString port)
{
    QSqlQuery query(db);
    query.exec("UPDATE ports SET status = 'busy' WHERE number =" + port);
}


