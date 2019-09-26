#include "mainwindow.h"
#include <QApplication>
#include <tests.h>

int main(int argc, char *argv[])
{
    testAll();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
