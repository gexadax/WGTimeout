// mainwindow.cpp
#include "settings.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "connect.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QString server, user, password;
    int port;
    DialogSettings::loadSettings("settings.ini", server, user, password, port);

    // Connect to SSH
    SSHConnector sshConnector;
    sshConnector.connectToSSH(server.toStdString(), user.toStdString(), password.toStdString(), port);
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

