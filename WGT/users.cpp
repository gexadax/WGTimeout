//users.cpp
#include "users.h"

QStringList users::getUserNames(const SSHConnector& sshConnector)
{
    // Execute the command remotely
    std::string command = "ls configs | awk '{print $1}' | xargs -I {} basename {} .conf";
    std::string result = sshConnector.executeCommand(command);

    // Process the result and convert it to QStringList
    QStringList userNames = QString::fromStdString(result).split('\n', QString::SkipEmptyParts);

    return userNames;
}

void users::displayOfRemainingDays(const SSHConnector& sshConnector, const QString& username, QSpinBox* spinBox)
{
    // Execute the command remotely
    std::string command = "echo $(($(crontab -l | grep " + username.toStdString() + " | grep -oP '\\*/\\K\\d+') - ((($(date +%s) - $(crontab -l | grep " + username.toStdString() + " | awk -F'#' '{print $2}' | tr -d ' ')) / 60))))";
    std::string result = sshConnector.executeCommand(command);

    // Process the result and update the spinBoxLimitDays
    int remainingDays = QString::fromStdString(result).toInt();
    spinBox->setValue(remainingDays);
}
