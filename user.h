#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isOnline READ isOnline)
    Q_PROPERTY(QString address READ address)
public:
    explicit User(QString address, QObject *parent = nullptr);
    bool _isOnline=false;
    QString _address;

    bool isOnline();
    QString address();
signals:

public slots:
};

#endif // USER_H
