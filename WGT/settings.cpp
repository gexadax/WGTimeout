// settings.cpp
#include "mainwindow.h"
#include "settings.h"
#include "ui_settings.h"
#include <QSettings>

void saveSettings(const QString& filePath, const QString& hostname, const QString& username, const QString& password, int port) {
    QSettings settings(filePath, QSettings::IniFormat);
    settings.setValue("hostname", hostname);
    settings.setValue("username", username);
    settings.setValue("password", password);
    settings.setValue("port", port);
}

void DialogSettings::loadSettings(const QString& filePath, QString& hostname, QString& username, QString& password, int& port) {
    QSettings settings(filePath, QSettings::IniFormat);
    hostname = settings.value("hostname").toString();
    username = settings.value("username").toString();
    password = settings.value("password").toString();
    port = settings.value("port").toInt();
}

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);

    // Load settings when creating the dialog
    QString hostname, username, password;
    int port;
    loadSettings("settings.ini", hostname, username, password, port);
    ui->textEdit_Hostname->setPlainText(hostname);
    ui->textEdit_Username->setPlainText(username);
    ui->lineEdit_Password->setText(password);
    ui->spinBox_Port->setValue(port);
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

std::string DialogSettings::getHostname() const {
    return ui->textEdit_Hostname->toPlainText().toStdString();
}

std::string DialogSettings::getUsername() const {
    return ui->textEdit_Username->toPlainText().toStdString();
}

std::string DialogSettings::getPassword() const {
    return ui->lineEdit_Password->text().toStdString();
}

int DialogSettings::getPort() const {
    return ui->spinBox_Port->value();
}

void DialogSettings::executeSSHCommand(const std::string& command) {
    // Executing SSH command
    LIBSSH2_CHANNEL* channel = libssh2_channel_open_session(sshConnector.getSession());
    if (channel) {
        if (libssh2_channel_exec(channel, command.c_str()) == 0) {
            // Waiting for command execution to complete
            char buffer[4096];
            int nbytes;
            do {
                nbytes = libssh2_channel_read(channel, buffer, sizeof(buffer));
                if (nbytes > 0) {
                    // Processing command output if needed
                }
            } while (nbytes > 0);
        } else {
            // Handling the case of a failed command execution
            std::cerr << "Failed to execute SSH command: " << command << std::endl;
            // Display error message or take other necessary actions
        }

        // Closing the channel after command execution
        libssh2_channel_free(channel);
    } else {
        // Handling the case when the channel failed to open
        std::cerr << "Failed to open SSH channel." << std::endl;
        // Display error message or take other necessary actions
    }
}

void DialogSettings::on_buttonBox_settings_accepted()
{
    // Get values entered in the dialog
    std::string hostname = this->getHostname();
    std::string username = this->getUsername();
    std::string password = this->getPassword();
    int port = this->getPort();

    // Save settings to file
    saveSettings("settings.ini",
                 QString::fromStdString(hostname),
                 QString::fromStdString(username),
                 QString::fromStdString(password),
                 port);

    // Connect using the entered data
    SSHConnector sshConnector;
    if (sshConnector.connectToSSH(hostname, username, password, port)) {
        // Connection successful
        std::cout << "SSH Connection successful" << std::endl;

        // Perform SSH operations...

        // Open the main window
        MainWindow mainWindow;
        mainWindow.show();
        this->accept(); // Close the settings dialog
    } else {
        // Connection error
        std::cerr << "SSH Connection failed" << std::endl;
    }
}
