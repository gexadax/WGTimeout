// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStringListModel>
#include <QMainWindow>
#include "users.h"
#include <QFile>

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
    void onListViewUserClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QString sshCommandResult;
    SSHConnector sshConnector;
    void displayUserNames(const SSHConnector& sshConnector);
    SSHConnector checkAndOpenSettingsDialog();
};

#endif // MAINWINDOW_H
