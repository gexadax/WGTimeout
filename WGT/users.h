// users.h
#ifndef USERS_H
#define USERS_H

#include <QStringList>
#include "connect.h"

class users
{
public:
    static QStringList getUserNames(const SSHConnector& sshConnector);
};

#endif // USERS_H
