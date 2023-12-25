// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStandardItemModel>
#include <QStringListModel>
#include <QStandardItem>
#include <QMainWindow>
#include <QFile>
#include "connect.h"
#include "users.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
    void on_actionSettings_triggered();

private:
    Ui::MainWindow *ui;
    QString sshCommandResult;
    void displayUserNames(const SSHConnector& sshConnector);
    SSHConnector checkAndOpenSettingsDialog();
};

#endif // MAINWINDOW_H
