// users.h
#ifndef USERS_H
#define USERS_H

#include <QStringList>
#include <QSpinBox>
#include "connect.h"

class users
{
public:
    static QStringList getUserNames(const SSHConnector& sshConnector);
    static void displayOfRemainingDays(const SSHConnector& sshConnector,
                                       const QString& username,
                                       QSpinBox* spinBox);
};

#endif // USERS_H
