// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStringListModel>
#include "./ui_mainwindow.h"
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
    void on_pushButtonAddUser_clicked();
    void on_pushButtonDeleteUser_clicked();
    void on_pushButtonActivateUser_clicked();
    void onListViewUserClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    void displayUserNames();
    QString sshCommandResult;
    SSHConnector sshConnector;
    SSHConnector checkAndOpenSettingsDialog();
};

#endif // MAINWINDOW_H
