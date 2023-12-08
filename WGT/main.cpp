//main.cpp
#include "mainwindow.h"
#include "settings.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Путь к директории программы
    QString programDir = QCoreApplication::applicationDirPath();

    // Полный путь к файлу settings.ini
    QString settingsFilePath = programDir + "/settings.ini";

    // Проверяем наличие файла
    if (!QFile::exists(settingsFilePath)) {
        // Файл не существует, открываем окно DialogSettings
        DialogSettings dialogSettings;
        if (dialogSettings.exec() == QDialog::Accepted) {
            // Принято, продолжаем выполнение программы
            // Открываем главное окно
            MainWindow mainWindow;
            mainWindow.show();
            return a.exec();
        } else {
            // Отменено или закрыто, обрабатываем соответственно
            return 0;
        }
    } else {
        // Файл существует, продолжаем выполнение программы
        // Открываем главное окно
        MainWindow mainWindow;
        mainWindow.show();
        return a.exec();
    }
}
