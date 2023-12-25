// mainwindow.cpp
#include "settings.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "connect.h"
#include "users.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Connect to SSH and load settings
    SSHConnector sshConnector = checkAndOpenSettingsDialog();  // Receive the SSHConnector instance

    // Display user names in the listViewUsers widget
    displayUserNames(sshConnector);
}

void MainWindow::displayUserNames(const SSHConnector& sshConnector) {
    // Get the list of user names using the getUserNames function
    QStringList userNames = users::getUserNames(sshConnector);

    // Create a model for the QListView
    QStringListModel *model = new QStringListModel(this);

    // Set the user names to the model
    model->setStringList(userNames);

    // Set the model to the QListView
    ui->listViewUsers->setModel(model);
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

SSHConnector MainWindow::checkAndOpenSettingsDialog() {
    // Path to the program directory
    QString programDir = QCoreApplication::applicationDirPath();

    // Full path to the settings.ini file
    QString settingsFilePath = programDir + "/settings.ini";

    // Check if the file exists
    if (!QFile::exists(settingsFilePath)) {
        // File does not exist, open the DialogSettings window
        DialogSettings dialogSettings;
        if (dialogSettings.exec() != QDialog::Accepted) {
            // Canceled or closed, handle accordingly
            this->close();
            return SSHConnector();  // Return an empty SSHConnector instance
        }
    }

    // Load settings after DialogSettings execution
    QString hostname, username, password;
    int port;
    DialogSettings::loadSettings("settings.ini", hostname, username, password, port);

    // Connect to SSH using the loaded settings
    SSHConnector sshConnector;
    if (sshConnector.connectToSSH(hostname.toStdString(), username.toStdString(), password.toStdString(), port)) {
        // Connection successful
        std::cout << "SSH Connection successful" << std::endl;

        // Perform SSH operations...

        // Open the main window
        this->show();
    } else {
        // Connection error
        std::cerr << "SSH Connection failed" << std::endl;
        this->close();
        return SSHConnector();  // Return an empty SSHConnector instance
    }

    return sshConnector;  // Return the SSHConnector instance
}
