#ifndef UIBACKEND_H
#define UIBACKEND_H

#include <QQmlListProperty>
#include <QObject>
#include <QFile>
#include <QDebug>
#include <QMap>
#include <QTcpSocket>
#include <QTcpServer>
#include <QtSql>
#include <QTime>
#include <QThread>
#include <QDataStream>



#include "user.h"

class UiBackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList comboList READ comboList NOTIFY comboListChanged)
    Q_PROPERTY(QStringList portList READ portList NOTIFY portListChanged)
    Q_PROPERTY(QStringList messages READ messages NOTIFY messagesChanged)
    Q_PROPERTY(QQmlListProperty<User> users READ users NOTIFY usersChanged)
public:
    explicit UiBackEnd(QObject *parent = nullptr);
    Q_INVOKABLE void destructor();
    QStringList* _comboList;
    QStringList* _messages;
    QStringList* _portList;

    QStringList comboList();
    QStringList messages();
    QStringList portList();

    QQmlListProperty<User> users();
    int userCount() const;
    User *user(int) const;
private:
    void connectToDB();
    void getPortsFromDB();
    void updateDB(QString port);
    QSqlDatabase db;
    QString thisPort;
    QList<User *> _users;
public slots:
    void slotNewUserOnline(QString addr);
    void slotGetInfoMessage(QString txt);
    void slotLogout();
    void slotUserDisconnected(QString address);
signals:
    void messagesChanged(QStringList messages);
    void comboListChanged();
    void portListChanged();
    void usersChanged();
    void logout();
    void signalStartServer(QStringList *listOfPorts,QString thisPort);
public slots:
    void slotStartServer(QString port);
};


#endif // UIBACKEND_H
