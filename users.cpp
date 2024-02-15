// users.cpp
#include "users.h"

void users::deleteUser(const SSHConnector &sshConnector, const QString &username)
{
    // Build the command to remove the PiVPN user, associated configurations, and crontab entries
    std::string deleteCommand = "crontab -l | grep -v '" + username.toStdString()
                                + "' | crontab - && "
                                  "rm ~/configs/"
                                + username.toStdString()
                                + ".* && "
                                  "sudo -S pivpn -r -y "
                                + username.toStdString();

    // Execute the combined command remotely
    sshConnector.executeCommand(deleteCommand);

    // Delete the QR code file from the local directory
    std::string localQRCodePath = QCoreApplication::applicationDirPath().toStdString() + "/"
                                  + username.toStdString() + "_config.png";
    std::remove(localQRCodePath.c_str());

    std::cout << "User '" << username.toStdString() << "' deleted successfully." << std::endl;
}

QStringList users::getUserNames(const SSHConnector &sshConnector)
{
    // Execute the command remotely
    std::string command = "ls configs | awk '{print $1}' | xargs -I {} basename {} .conf";
    std::string result = sshConnector.executeCommand(command);

    // Process the result and convert it to QStringList
    QStringList userNames = QString::fromStdString(result).split('\n', QString::SkipEmptyParts);

    return userNames;
}

void users::displayOfRemainingDays(const SSHConnector &sshConnector,
                                   const QString &username,
                                   QSpinBox *spinBox)
{
    // Execute the command remotely
    std::string command = "echo $(($(crontab -l | grep " + username.toStdString()
                          + " | grep -oP '\\*/\\K\\d+') - ((($(date +%s) - $(crontab -l | grep "
                          + username.toStdString()
                          + " | awk -F'#' '{print $2}' | tr -d ' ')) / 60))))";
    std::string result = sshConnector.executeCommand(command);

    // Process the result and update the spinBoxLimitDays
    int remainingDays = QString::fromStdString(result).toInt();
    spinBox->setValue(remainingDays);
}

void users::createCronTask(const SSHConnector &sshConnector,
                           const QString &username,
                           const QString &valueInDays)
{
    // Build the command to add a cron task using the value from spinBoxLimitDays
    std::string command = "(crontab -l ; echo \"* * */" + valueInDays.toStdString()
                          + " * * /bin/mv ~/configs/" + username.toStdString() + ".conf ~/configs/"
                          + username.toStdString() + ".timeout && " + "(crontab -l | grep -v '"
                          + username.toStdString() + ".conf' | crontab -) && sudo -S pivpn -off -y "
                          + username.toStdString() + " # $(date +\\%s)\") | crontab -";
    // Execute the command remotely
    sshConnector.executeCommand(command);
}

void users::createPiVPNUser(const SSHConnector &sshConnector,
                            const QString &username,
                            const QString &password)
{
    // Build the command to add a user in PiVPN using sudo and passing the password
    std::string command = "echo '" + password.toStdString() + "' | sudo -S pivpn -a -n "
                          + username.toStdString();
    sshConnector.executeCommand(command);
}

void users::generateQRCode(const SSHConnector &sshConnector, const QString &username)
{
    // Construct the qrencode command without using echo
    std::string command = "qrencode -t PNG -o ~/configs/" + username.toStdString()
                          + "_config.png < ~/configs/" + username.toStdString() + ".conf";

    // Execute the command remotely
    std::string result = sshConnector.executeCommand(command);
}

void users::moveFileFromServerToLocal(const SSHConnector &sshConnector, const QString &username)
{
    // Construct source path
    std::string sourcePath = "~/configs/" + username.toStdString() + "_config.png";

    // Download the file and write the content to the local file
    std::string downloadCommand = "cat " + sourcePath;
    std::string result = sshConnector.executeCommand(downloadCommand);

    // Write the content to the local file
    QFile localFile(QCoreApplication::applicationDirPath() + "/" + username + "_config.png");
    if (localFile.open(QIODevice::WriteOnly)) {
        localFile.write(result.c_str(), result.size());
        localFile.close();
        std::cout << "File moved successfully to local directory." << std::endl;

        // Remove the file from the remote server
        std::string removeCommand = "rm " + sourcePath;
        sshConnector.executeCommand(removeCommand);
    } else {
        std::cerr << "Error moving the file to local directory." << std::endl;
    }
}

void users::activateUser(const SSHConnector &sshConnector,
                         const QString &username,
                         const QString &valueInDays)
{
    // Build the command to rename the timeout file to conf file
    std::string renameCommand = "mv ~/configs/" + username.toStdString() + ".timeout ~/configs/"
                                + username.toStdString() + ".conf";
    sshConnector.executeCommand(renameCommand);

    // Build the command to add a cron task using the value from spinBoxLimitDays
    std::string command = "(crontab -l ; echo \"* * */" + valueInDays.toStdString()
                          + " * * /bin/mv ~/configs/" + username.toStdString() + ".conf ~/configs/"
                          + username.toStdString() + ".timeout && " + "(crontab -l | grep -v '"
                          + username.toStdString() + ".conf' | crontab -) && sudo -S pivpn -off -y "
                          + username.toStdString() + " # $(date +\\%s)\") | crontab -";
    // Execute the command remotely
    sshConnector.executeCommand(command);

    // Build the command to activate the user in PiVPN
    std::string activateCommand = "sudo -S pivpn -on -y " + username.toStdString();
    sshConnector.executeCommand(activateCommand);

    std::cout << "User '" << username.toStdString() << "' activated successfully." << std::endl;
}
