// users.h
#ifndef USERS_H
#define USERS_H

#include "connect.h"
#include <QStringList>

class users
{
public:
    static QStringList getUserNames(const SSHConnector& sshConnector);
};

#endif // USERS_H
