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
        //ui->portBox->addItems(*listOfPorts);
        //connect(ui->startbtn, SIGNAL(clicked()),this, SLOT(slotStartServer()));
}

void UiBackEnd::slotGetInfoMessage(QString txt)
{
     //ui->info->append(txt);
}

void UiBackEnd::slotStartServer()
{
    //QString thisPort = ui->portBox->currentText();
    //emit signalStartServer(listOfPorts,thisPort);
}

UiBackEnd::~UiBackEnd()
{
}
