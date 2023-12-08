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

private slots:

    void on_buttonBox_accepted();

private:
    Ui::DialogSettings *ui;
    void executeSSHCommand(SSHConnector& sshConnector, const std::string& command);
};

#endif // SETTINGS_H
