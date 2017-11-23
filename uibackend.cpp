#include "uibackend.h"
#include "networkbackend.h"
UiBackEnd::UiBackEnd(QObject *parent) : QObject(parent)
{
    listOfPorts = new QStringList();
    QFile file("://ports.txt");
        if(file.open(QIODevice::ReadOnly |QIODevice::Text))
        {
            while(!file.atEnd())
            {
                QString str = file.readLine();
                str.remove(str.length()-1,1);
                listOfPorts->append(str);
            }
        }
        else
        {
            qDebug()<< "can't open file";
        }
}
const QStringList UiBackEnd::comboList()
{
    return *listOfPorts;
}
void UiBackEnd::slotGetInfoMessage(QString txt)
{
     //ui->info->append(txt);
}

void UiBackEnd::slotStartServer(QString port)
{
    QString thisPort = port;
    qDebug()<<"START SERVER WITH PORT: "<<port;
    //emit signalStartServer(listOfPorts,thisPort);
}

UiBackEnd::~UiBackEnd()
{
}
