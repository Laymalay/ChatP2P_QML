#include "uibackend.h"
#include "networkbackend.h"
UiBackEnd::UiBackEnd(QObject *parent) : QObject(parent)
{
    _comboList = new QStringList();
    _messages = new QStringList();
    QFile file("://ports.txt");
        if(file.open(QIODevice::ReadOnly |QIODevice::Text))
        {
            while(!file.atEnd())
            {
                QString str = file.readLine();
                str.remove(str.length()-1,1);
                _comboList->append(str);
            }
        }
        else
        {
            qDebug()<< "can't open file";
        }
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
    QString thisPort = port;
    qDebug()<<"START SERVER WITH PORT: "<<port;
    emit signalStartServer(_comboList,thisPort);
}

UiBackEnd::~UiBackEnd()
{
}
