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
    Q_PROPERTY(QStringList comboList READ comboList NOTIFY comboListChanged)
public:
    explicit UiBackEnd(QObject *parent = nullptr);
    ~UiBackEnd();
    QStringList* listOfPorts;
    QStringList* messages;
    const QStringList comboList();
private:

public slots:
     void slotGetInfoMessage(QString txt);
signals:
    void comboListChanged();
    void signalStartServer(QStringList *listOfPorts,QString thisPort);
public slots:
    void slotStartServer(QString port);
};


#endif // UIBACKEND_H
