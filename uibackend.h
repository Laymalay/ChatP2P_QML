#ifndef UIBACKEND_H
#define UIBACKEND_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QMap>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTime>
#include <QThread>
#include <QDataStream>

class UiBackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList comboList READ comboList)
public:
    explicit UiBackEnd(QObject *parent = nullptr);
    ~UiBackEnd();
    QStringList* listOfPorts;
    const QStringList comboList();
private:

public slots:
     void slotGetInfoMessage(QString txt);
signals:
    void signalStartServer(QStringList *listOfPorts,QString thisPort);
private slots:
    void slotStartServer();
};


#endif // UIBACKEND_H
