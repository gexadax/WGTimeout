// mainwindow.cpp
#include "settings.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "connect.h"
#include "users.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QString hostname, username, password;
    int port;
    DialogSettings::loadSettings("settings.ini", hostname, username, password, port);

    // Connect to SSH
    SSHConnector sshConnector;
    if (sshConnector.connectToSSH(hostname.toStdString(), username.toStdString(), password.toStdString(), port)) {
        // Connection successful
        std::cout << "SSH Connection successful" << std::endl;

        // Get the list of user names
        std::string command = "ls configs | awk '{print $1}' | xargs -I {} basename {} .conf";
        sshCommandResult = QString::fromStdString(sshConnector.executeCommand(command));
        std::cout << "SSH Command Result: " << sshCommandResult.toStdString() << std::endl;
        std::cout << "SSH Command executed successfully." << std::endl;

        // Create a model for the QListView
        QStringListModel *model = new QStringListModel(this);

        // Add user names to the model
        QStringList userNames = sshCommandResult.split('\n', QString::SkipEmptyParts);
        model->setStringList(userNames);

        // Set the model to the QListView
        ui->listViewUsers->setModel(model);
    } else {
        // Connection error
        std::cerr << "SSH Connection failed" << std::endl;
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionSettings_triggered()
{
    // Create an instance of the DialogSettings class
    DialogSettings settingsDialog(this); // Assuming DialogSettings is a QDialog

    // Show the settings window
    settingsDialog.exec();
}

