// mainwindow.cpp
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect to SSH and load settings
    sshConnector = checkAndOpenSettingsDialog();

    // Display user names in the listViewUsers widget
    displayUserNames();

    connect(ui->listViewUsers, &QListView::clicked, this, &MainWindow::onListViewUserClicked);
}

void MainWindow::displayUserNames()
{
    // Get the list of user names using the getUserNames function
    QStringList userNames = users::getUserNames(sshConnector);

    // Create a model for the QListView
    QStringListModel *model = new QStringListModel(this);

    // Set the user names to the model
    model->setStringList(userNames);

    // Set the model to the QListView
    ui->listViewUsers->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionSettings_triggered()
{
    // Create an instance of the DialogSettings class
    DialogSettings settingsDialog(this);

    // Show the settings window
    settingsDialog.exec();
}

SSHConnector MainWindow::checkAndOpenSettingsDialog()
{
    // Path to the program directory
    QString programDir = QCoreApplication::applicationDirPath();

    // Full path to the settings.ini file
    QString settingsFilePath = programDir + "/settings.ini";

    // Create an instance of the DialogSettings class
    DialogSettings settingsDialog;

    while (true) {
        // Check if the file exists
        if (!QFile::exists(settingsFilePath)) {
            // File does not exist, open the DialogSettings window
            if (settingsDialog.exec() != QDialog::Accepted) {
                // Canceled or closed, handle accordingly
                this->close();
                return SSHConnector();
            }
        }

        // Load settings after DialogSettings execution
        QString hostname, username, password;
        int port;
        DialogSettings::loadSettings("settings.ini", hostname, username, password, port);

        // Connect to SSH using the loaded settings
        SSHConnector sshConnector;
        if (sshConnector.connectToSSH(hostname.toStdString(),
                                      username.toStdString(),
                                      password.toStdString(),
                                      port)) {
            // Connection successful
            std::cout << "SSH Connection successful" << std::endl;

            // Open the main window
            this->show();

            // Configure sudo with the entered username and command path
            if (!settingsDialog.addSudoEntry(sshConnector,
                                             username.toStdString(),
                                             "/usr/local/bin/pivpn")) {
                // Handle the case where addSudoEntry fails
                std::cerr << "Failed to configure sudo." << std::endl;
                this->close();
                return SSHConnector();
            }

            return sshConnector;
        } else {
            // Connection error
            std::cerr << "SSH Connection failed" << std::endl;
            if (settingsDialog.exec() != QDialog::Accepted) {
                this->close();
                return SSHConnector();
            }
        }
    }
}

void MainWindow::onListViewUserClicked(const QModelIndex &index)
{
    // Get the selected username from the clicked item
    QString selectedUsername = index.data(Qt::DisplayRole).toString();

    // Remove the ".timeout" extension from the selected username
    selectedUsername.replace(".timeout", "");

    // Update the lineEditAddUser with the selected username
    ui->lineEditAddUser->setText(selectedUsername);

    // Call the function to get remaining days and update the spinBoxLimitDays
    users::displayOfRemainingDays(sshConnector, selectedUsername, ui->spinBoxLimitDays);
}

void MainWindow::on_pushButtonAddUser_clicked()
{
    // Get the username from lineEditAddUser
    QString username = ui->lineEditAddUser->text().trimmed();

    // Get the password from settings
    QString password = DialogSettings::getPasswordFromSettings();

    // Optionally, get the value from spinBoxLimitDays
    QString limitDays = QString::number(ui->spinBoxLimitDays->value());

    // Execute the crontab command with the updated createCronTask signature
    users::createCronTask(sshConnector, username, limitDays);

    // Execute the pivpn command
    users::createPiVPNUser(sshConnector, username, password);

    // Optionally, update the usernames in the listViewUsers widget
    displayUserNames();

    // Optionally, update spinBoxLimitDays for the added user
    users::displayOfRemainingDays(sshConnector, username, ui->spinBoxLimitDays);

    // Generate QR code
    users::generateQRCode(sshConnector, username);

    // Assuming sshConnector is an instance of your SSHConnector class
    users::moveFileFromServerToLocal(sshConnector, username);
}

void MainWindow::on_pushButtonDeleteUser_clicked()
{
    // Get the username from lineEditAddUser
    QString username = ui->lineEditAddUser->text().trimmed();

    // Call the deleteUser function
    users::deleteUser(sshConnector, username);

    // Optionally, update the usernames in the listViewUsers widget
    displayUserNames();

    // Optionally, update spinBoxLimitDays for the added user
    users::displayOfRemainingDays(sshConnector, username, ui->spinBoxLimitDays);
}

void MainWindow::on_pushButtonActivateUser_clicked()
{
    // Get the username from lineEditAddUser
    QString username = ui->lineEditAddUser->text().trimmed();

    // Optionally, get the value from spinBoxLimitDays
    QString limitDays = QString::number(ui->spinBoxLimitDays->value());

    // Activate the user
    users::activateUser(sshConnector, username, limitDays);

    // Optionally, update the usernames in the listViewUsers widget
    displayUserNames();

    // Optionally, update spinBoxLimitDays for the added user
    users::displayOfRemainingDays(sshConnector, username, ui->spinBoxLimitDays);

    // Generate QR code
    users::generateQRCode(sshConnector, username);

    // Assuming sshConnector is an instance of your SSHConnector class
    users::moveFileFromServerToLocal(sshConnector, username);
}
