// settings.h
#ifndef SETTINGS_H
#define SETTINGS_H

#include "connect.h"
#include <QDialog>

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT
public:
    explicit DialogSettings(QWidget *parent = nullptr);
    ~DialogSettings();

    std::string getHostname() const;
    std::string getUsername() const;
    std::string getPassword() const;
    int getPort() const;

    static void loadSettings(const QString& filePath,
                             QString& hostname,
                             QString& username,
                             QString& password,
                             int& port);
private slots:
    void executeSSHCommand(const std::string& command);
    void on_buttonBox_settings_accepted();
private:
    Ui::DialogSettings *ui;
    SSHConnector sshConnector;
};

#endif // SETTINGS_H
