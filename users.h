// users.h

#ifndef USERS_H
#define USERS_H

#include <QStringList>
#include <QSpinBox>
#include <QFile>
#include <QCoreApplication>
#include "connect.h"
#include "settings.h"

class users
{
public:
    static void createCronTask(const SSHConnector& sshConnector,
                               const QString& username,
                               const QString& valueInMinutes);
    static QStringList getUserNames(const SSHConnector& sshConnector);
    static void createPiVPNUser(const SSHConnector& sshConnector,
                                const QString& username,
                                const QString& password);
    static void activateUser(const SSHConnector& sshConnector,
                                const QString& username,
                                const QString& password);
    static void displayOfRemainingDays(const SSHConnector& sshConnector,
                                       const QString& username,
                                       QSpinBox* spinBox);
    static void generateQRCode(const SSHConnector& sshConnector,
                               const QString& username);
    static void moveFileFromServerToLocal(const SSHConnector& sshConnector,
                                          const QString& username);
    static void deleteUser(const SSHConnector& sshConnector,
                           const QString& username);
};

#endif // USERS_H
