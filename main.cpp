// main.cpp
#include "mainwindow.h"
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator myappTranslator;
    myappTranslator.load("wgt_ru.qm");
    a.installTranslator(&myappTranslator);

    QTranslator qtTranslator;
    qtTranslator.load("wgt_ru.qm");
    a.installTranslator(&qtTranslator);

    MainWindow mainWindow;
    mainWindow.show();
    return a.exec();
}
