// settings.cpp
#include "mainwindow.h"
#include "settings.h"
#include "ui_settings.h"
#include <QSettings>

void saveSettings(const QString& filePath, const QString& server, const QString& user, const QString& password, int port) {
    QSettings settings(filePath, QSettings::IniFormat);
    settings.setValue("Server", server);
    settings.setValue("User", user);
    settings.setValue("Password", password);
    settings.setValue("Port", port);
}

void loadSettings(const QString& filePath, QString& server, QString& user, QString& password, int& port) {
    QSettings settings(filePath, QSettings::IniFormat);
    server = settings.value("Server").toString();
    user = settings.value("User").toString();
    password = settings.value("Password").toString();
    port = settings.value("Port").toInt();
}

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);

    // Загружаем настройки при создании диалога
    QString server, user, password;
    int port;
    loadSettings("settings.ini", server, user, password, port);
    ui->textEdit->setPlainText(server);
    ui->textEdit_2->setPlainText(user);
    ui->lineEdit->setText(password);
    ui->spinBox->setValue(port);
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

std::string DialogSettings::getServer() const {
    return ui->textEdit->toPlainText().toStdString();
}

std::string DialogSettings::getUser() const {
    return ui->textEdit_2->toPlainText().toStdString();
}

std::string DialogSettings::getPassword() const {
    return ui->lineEdit->text().toStdString();
}

int DialogSettings::getPort() const {
    return ui->spinBox->value();
}

void DialogSettings::executeSSHCommand(SSHConnector& sshConnector, const std::string& command) {
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
                // Process command output if needed
            }
        } while (nbytes > 0);

        // Closing a channel after executing a command
        libssh2_channel_free(channel);
    }
}

void DialogSettings::on_buttonBox_accepted()
{
    // Получаем введенные значения из диалога
    std::string hostname = this->getServer();
    std::string username = this->getUser();
    std::string password = this->getPassword();
    int port = this->getPort();

    // Сохраняем настройки в файл
    saveSettings("settings.ini", QString::fromStdString(hostname), QString::fromStdString(username), QString::fromStdString(password), port);

    // Далее осуществляем подключение с использованием полученных данных
    SSHConnector sshConnector;
    if (sshConnector.connectToSSH(hostname, username, password, port)) {
        // Подключение успешно
        std::cout << "SSH Connection successful" << std::endl;

        // Выполняем SSH-операции...

        // Открываем главное окно
        MainWindow mainWindow;
        mainWindow.show();
        this->accept(); // Закрыть диалог настроек
    } else {
        // Ошибка подключения
        std::cerr << "SSH Connection failed" << std::endl;
    }
}
