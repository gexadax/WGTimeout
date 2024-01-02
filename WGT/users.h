// users.h
#ifndef USERS_H
#define USERS_H

#include <QStringList>
#include <QSpinBox>
#include "connect.h"

class users
{
public:
    static void createCronTask(const SSHConnector& sshConnector,
                               const QString& username,
                               const QString& cronExpression);
    static QStringList getUserNames(const SSHConnector& sshConnector);
    static void createPiVPNUser(const SSHConnector& sshConnector,
                                const QString& username,
                                const QString& password);
    static void displayOfRemainingDays(const SSHConnector& sshConnector,
                                       const QString& username,
                                       QSpinBox* spinBox);
};

#endif // USERS_H
