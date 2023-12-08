// main.cpp
#include "mainwindow.h"
#include "settings.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Path to the program directory
    QString programDir = QCoreApplication::applicationDirPath();

    // Full path to the settings.ini file
    QString settingsFilePath = programDir + "/settings.ini";

    // Check if the file exists
    if (!QFile::exists(settingsFilePath)) {
        // File does not exist, open the DialogSettings window
        DialogSettings dialogSettings;
        if (dialogSettings.exec() == QDialog::Accepted) {
            // Accepted, continue program execution
            // Open the main window
            MainWindow mainWindow;
            mainWindow.show();
            return a.exec();
        } else {
            // Canceled or closed, handle accordingly
            return 0;
        }
    } else {
        // File exists, continue program execution
        // Open the main window
        MainWindow mainWindow;
        mainWindow.show();
        return a.exec();
    }
}
