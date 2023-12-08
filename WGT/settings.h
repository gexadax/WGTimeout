//settings.h
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

    std::string getServer() const;
    std::string getUser() const;
    std::string getPassword() const;
    int getPort() const;

    static void loadSettings(const QString& filePath,
                             QString& server,
                             QString& user,
                             QString& password,
                             int& port);
private slots:
    void on_buttonBox_accepted();
    void executeSSHCommand(const std::string& command);

private:
    Ui::DialogSettings *ui;
    SSHConnector sshConnector;
};

#endif // SETTINGS_H
