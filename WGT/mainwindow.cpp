// mainwindow.cpp
#include "settings.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "connect.h"
#include "users.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Call the function to check and open the settings dialog
    checkAndOpenSettingsDialog(this);

    // Connect to SSH
    QString hostname, username, password;
    int port;
    DialogSettings::loadSettings("settings.ini", hostname, username, password, port);

    SSHConnector sshConnector;
    if (sshConnector.connectToSSH(hostname.toStdString(), username.toStdString(), password.toStdString(), port)) {
        // Connection successful
        std::cout << "SSH Connection successful" << std::endl;

        // Get the list of user names using the getUserNames function
        QStringList userNames = users::getUserNames(sshConnector);

        // Create a model for the QListView
        QStringListModel *model = new QStringListModel(this);

        // Set the user names to the model
        model->setStringList(userNames);

        // Set the model to the QListView
        ui->listViewUsers->setModel(model);
    } else {
        // Connection error
        std::cerr << "SSH Connection failed" << std::endl;
        // Handle the failure if necessary
        this->close();
        return;
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

