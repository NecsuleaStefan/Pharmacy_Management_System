#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int b=12;
    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
