#include "uibackend.h"
#include "networkbackend.h"
UiBackEnd::UiBackEnd(QObject *parent) : QObject(parent)
{
    _comboList = new QStringList();
    _messages = new QStringList();
    _portList = new QStringList();

    connectToDB();
    getPortsFromDB();
}
QStringList UiBackEnd::comboList()
{
    return *_comboList;
}
QStringList UiBackEnd::portList()
{
    return *_portList;
}


QStringList UiBackEnd::messages()
{
    return *_messages;
}
QQmlListProperty<User> UiBackEnd::users()
{
    return QQmlListProperty<User>(this, _users);
}
int UiBackEnd::userCount() const
{
    return _users.count();
}

User *UiBackEnd::user(int index) const
{
    return _users.at(index);
}


void UiBackEnd::slotGetInfoMessage(QString txt)
{
    _messages->append(txt);
    emit messagesChanged(messages());
}

void UiBackEnd::slotLogout()
{
    User* u = new User(thisPort);
    _users.append(u);
    emit usersChanged();
    emit logout();
    QSqlQuery query(db);
    query.exec("UPDATE ports SET status = 'free' WHERE number =" + thisPort);
    thisPort = "";
}

void UiBackEnd::slotUserDisconnected(QString address)
{
    for(int i=0;i<_users.size();i++){
        if(_users.at(i)->address() == address){
            _users.at(i)->setStatus(false);
            emit usersChanged();
        }
    }
}

void UiBackEnd::slotStartServer(QString port)
{
    updateDB(port);
    thisPort = port;
    for(int i=0;i<_users.size();i++){
        if(_users.at(i)->address() == thisPort){
            _users.removeAt(i);
            emit usersChanged();
        }
    }
    qDebug()<<"START SERVER WITH PORT: "<<port;
    emit signalStartServer(_portList,thisPort);
}

void UiBackEnd::destructor()
{
    qDebug()<<"Destructor";
    if(thisPort!=""){
        QSqlQuery query(db);
        query.exec("UPDATE ports SET status = 'free' WHERE number =" + thisPort);
    }
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
       _portList->append(port);
       User* user = new User(port);
       _users.append(user);
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
    qDebug()<<"UPDATE ports SET status = 'busy' WHERE number =" + port;
}

void UiBackEnd::slotNewUserOnline(QString addr)
{
    qDebug()<<addr;
    for(int i=0;i<_users.size();i++){
        if (_users.at(i)->address() == addr){
            _users.at(i)->setStatus(true);
            _users.at(i)->statusChanged();
        }
        qDebug()<<_users.at(i)->address() << " "<<_users.at(i)->isOnline();
    }
}


