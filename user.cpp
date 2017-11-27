#include "user.h"

User::User(QString address, QObject *parent) : QObject(parent)
{
    this->_address = address;
}

bool User::isOnline()
{
    return _isOnline;
}

QString User::address()
{
    return _address;
}
