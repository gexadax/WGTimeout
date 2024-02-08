// settings.h
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QDialog>
#include "connect.h"
#include "ui_settings.h"

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    ~DialogSettings();
    int getPort() const;
    std::string getHostname() const;
    std::string getUsername() const;
    std::string getPassword() const;
    static QString getPasswordFromSettings();
    static void loadSettings(const QString &filePath,
                             QString &hostname,
                             QString &username,
                             QString &password,
                             int &port);
    explicit DialogSettings(QWidget *parent = nullptr);
    bool addSudoEntry(const SSHConnector &sshConnector,
                       const std::string &username,
                       const std::string &commandPath);
private slots:
    void on_buttonBox_settings_accepted();
    void executeSSHCommand(const std::string &command);

private:
    Ui::DialogSettings *ui;
    SSHConnector sshConnector;
    static const int BUFFER_SIZE = 1024;
};

#endif // SETTINGS_H
