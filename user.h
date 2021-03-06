#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isOnline READ isOnline NOTIFY statusChanged)
    Q_PROPERTY(QString address READ address NOTIFY addressChanged)
public:
    explicit User(QString address, QObject *parent = nullptr);
    bool isOnline();
    QString address();
    void setStatus(bool stat);
private:
    bool _isOnline=false;
    QString _address;
    QString _name;
    int _id;
signals:
    void statusChanged();
    void addressChanged();
public slots:
};

#endif // USER_H
