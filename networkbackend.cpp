#include "uibackend.h"
#include "networkbackend.h"

NetworkBackEnd::NetworkBackEnd(QObject *parent) : m_nNextBlockSize(0)
{

    listOfPorts = new QStringList();
    server = new QTcpServer(this);
    portMap = new QMap<int,QTcpSocket*>();
    socketMap = new QMap<int,int>();

}

void NetworkBackEnd::process(){
    timer = new QTimer();
    timer->setInterval(2000);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotLookUpNewConnections()));
}

void NetworkBackEnd::slotDisconnected()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug()<<"--------------------Disconnected--------------------"<< socket->peerPort()<<socket->state();
    emit socketDisconnected(QString::number(socketMap->key(socket->peerPort())));
    notConnectedYet.append(QString::number(socketMap->key(socket->peerPort())));
    socketMap->remove(socketMap->key(socket->peerPort()));

}

void NetworkBackEnd::slotStartServer(QStringList *_listOfPorts, QString _thisPort)
{

    listOfPorts = _listOfPorts;
    thisPort = _thisPort;
    notConnectedYet = *listOfPorts;
    notConnectedYet.removeAt(notConnectedYet.indexOf(thisPort));
    qDebug()<<"server: "+thisPort;
    emit sendInfoMessage("server "+ thisPort + " has started");

    if(!server->listen(QHostAddress::Any, thisPort.toInt())){
        qDebug()<<"unable to start the server:"+server->errorString();
        server->close();
        return;
    }
    connect(server, SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
    for(int i =0;i<notConnectedYet.size();i++)
        qDebug()<<notConnectedYet.at(i);
    timer->start();
}


void NetworkBackEnd::sendMsgToSocket(QTcpSocket* pSocket, const QString& str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    pSocket->write(arrBlock);
}


void NetworkBackEnd::slotNewConnection() {
    qDebug()<<"NEW CONNECTION";
    if (server->hasPendingConnections()){
        QTcpSocket* socket = server->nextPendingConnection();
        connect(socket, SIGNAL(disconnected()),this, SLOT(slotDisconnected()));
        connect(socket, SIGNAL(disconnected()),socket, SLOT(deleteLater()));
        connect(socket, SIGNAL(readyRead()),this, SLOT(slotReadyRead()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(slotError(QAbstractSocket::SocketError)));
        sendMsgToSocket(socket, "Response: Connected! " + thisPort +"  "+ QString::number(socket->peerPort()));
    }
}

void NetworkBackEnd::slotLogout()
{
    qDebug()<<"======================================DISCONNECT====================================";
    timer->stop();
    for(auto e: portMap->toStdMap()){
       QTcpSocket* s = e.second;
       qDebug()<<s->peerAddress()<<s->peerName()<<s->peerPort();
       s->disconnectFromHost();
       s->deleteLater();
    }
    server->close();
}

void NetworkBackEnd::slotReadSocket()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_5);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }
        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime time;
        QString str;
        in >> time >> str;
        QString strMessage = time.toString() + " " + "Client: " + str + " " + pClientSocket->peerPort();
        emit sendInfoMessage(strMessage);
        sendMsgToSocket(pClientSocket,thisPort);
        m_nNextBlockSize = 0;
     }
}
void NetworkBackEnd::slotConnected()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug()<<"Received the connected() signal" << socket->peerPort();
    emit sendInfoMessage("Received the connected() signal"+QString::number(socket->peerPort()));
}

void NetworkBackEnd::slotReadyRead()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_3);
    for(;;){
        if(!m_nNextBlockSize){
            if(socket->bytesAvailable() < sizeof(quint16)){
                break;
            }
            in >> m_nNextBlockSize;
        }
        if(socket->bytesAvailable()<m_nNextBlockSize){
            break;
        }
        QTime time;
        QString str;
        in>>time>>str;
        emit sendInfoMessage(time.toString()+" Server: "+str +" "+ QString::number(socket->peerPort()));
        socketMap->insert(str.toInt(),socket->peerPort());
        for(auto e : socketMap->toStdMap()){
            qDebug()<<e.first<<e.second;
        }
        m_nNextBlockSize = 0;
    }
}

void NetworkBackEnd::slotError(QAbstractSocket::SocketError err)
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug()<<socket->peerPort();
    QString strError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                         "the host wasn't found." :
                         err == QAbstractSocket::RemoteHostClosedError ?
                         "the remote host is closed." :
                         err == QAbstractSocket::ConnectionRefusedError ?
                         "the connection was refused." :
                         QString(socket->errorString())
            );
//   emit socketDisconnected(QString::number(socket->peerPort()));
    qDebug()<<strError;
// emit sendInfoMessage(strError);
}


void NetworkBackEnd::slotLookUpNewConnections(){
    for (int i=0;i<notConnectedYet.size();i++){
          qDebug()<<"slotLookUpNewConnections";
          QTime timer;
          timer.start();
          QTcpSocket* socket = new QTcpSocket();
          connect(socket, SIGNAL(connected()),this, SLOT(slotConnected()));
          connect(socket, SIGNAL(readyRead()),this, SLOT(slotReadSocket()));
          connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
                  this, SLOT(slotError(QAbstractSocket::SocketError)));
          socket->connectToHost("localhost",(notConnectedYet.at(i)).toInt());
          if(socket->waitForConnected(500000)){
              qDebug()<<thisPort+"+"+notConnectedYet.at(i)<< "Connected in"<<timer.elapsed();
              qDebug()<<"SOCKET CYCLE"<<thisPort<<socket->peerPort()<<socket;
              portMap->insert((notConnectedYet.at(i)).toInt(),socket);
              emit NewUserOnline(notConnectedYet.at(i));
              notConnectedYet.removeAt(i);
          }
          else{
              disconnect(socket, SIGNAL(connected()),this, SLOT(slotConnected()));
              disconnect(socket, SIGNAL(readyRead()),this, SLOT(slotReadSocket()));
              disconnect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
                      this, SLOT(slotError(QAbstractSocket::SocketError)));
              socket->deleteLater();
              qDebug()<<thisPort+"+"+notConnectedYet.at(i)<< "Not connected in"<<timer.elapsed();
          }
     }
}


NetworkBackEnd::~NetworkBackEnd()
{

}
