// mainwindow.cpp
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "connect.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::executeSSHCommand(SSHConnector& sshConnector, const std::string& command) {
    // Executing SSH command
    LIBSSH2_CHANNEL* channel = libssh2_channel_open_session(sshConnector.getSession());
    if (channel) {
        libssh2_channel_exec(channel, command.c_str());

        // Waiting for the command to complete execution
        char buffer[4096];
        int nbytes;
        do {
            nbytes = libssh2_channel_read(channel, buffer, sizeof(buffer));
            if (nbytes > 0) {
                // Process command output if need
            }
        } while (nbytes > 0);

        // Closing a channel after executing a command
        libssh2_channel_free(channel);
    }
}

void MainWindow::slot_pushButton_clicked() {
    SSHConnector sshConnector;

    std::string hostname = "server";
    std::string username = "user";
    std::string password = "password";
    int port = 22;

    if (sshConnector.connectToSSH(hostname, username, password, port)) {
        std::cout << "SSH Connection successful" << std::endl;

        // Perform your SSH operations here...
        executeSSHCommand(sshConnector, "touch 1111.txt");

        // Disconnect after SSH operations
        sshConnector.disconnectToSSH();
    } else {
        std::cerr << "SSH Connection failed" << std::endl;
    }
}
