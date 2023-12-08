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
