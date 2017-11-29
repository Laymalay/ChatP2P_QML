#ifndef NETWORKBACKEND_H
#define NETWORKBACKEND_H

#include <QObject>
#include <QTimer>

#include "uibackend.h"
class NetworkBackEnd : public QObject
{
    Q_OBJECT
public:
    explicit NetworkBackEnd(QObject *parent = nullptr);
    ~NetworkBackEnd();
    QStringList* listOfPorts;
    QString thisPort;
    QMap<int,QTcpSocket*>* portMap;
public slots:
    void process();
    void slotLogout();
    void slotStartServer(QStringList *listOfPorts, QString thisPort);

signals:
    void finished();
    void newConnection();
    void NewUserOnline(QString addr);
    void sendInfoMessage(QString txt);
    void socketDisconnected(QString address);

private:
    quint16 m_nNextBlockSize;
    QTcpServer* server;
    QTcpSocket* socket;
    QStringList notConnectedYet;
    QTimer* timer;
private:
    void sendMsgToSocket(QTcpSocket* pSocket, const QString& str);
    void startConnection();
private slots:
    void slotNewConnection();
    void slotReadSocket();
    void slotConnected();
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotLookUpNewConnections();
};

#endif // NETWORKBACKEND_H
