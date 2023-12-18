//users.cpp
#include "users.h"

QStringList users::getUserNames(const SSHConnector& sshConnector)
{
    // Execute the command remotely
    std::string command = "ll configs | awk '{print $9}' | xargs -I {} basename {} .conf";
    std::string result = sshConnector.executeCommand(command);

    // Process the result and convert it to QStringList
    QStringList userNames = QString::fromStdString(result).split('\n', QString::SkipEmptyParts);

    return userNames;
}
