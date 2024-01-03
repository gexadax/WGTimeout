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
    static void generateQRCode(const SSHConnector& sshConnector,
                               const QString& username);
    static void moveFileFromServerToLocal(const std::string& remoteFilePath,
                                          const SSHConnector& sshConnector);

};

#endif // USERS_H
