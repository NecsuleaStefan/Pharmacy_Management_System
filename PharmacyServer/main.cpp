#include "mainwindow.h"
#include "databasemanager.h"
#include "networkserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "--- Starting Pharmacy Server ---";

    DatabaseManager dbManager;
    if (!dbManager.initializeDatabase()) {
        qDebug() << "CRITICAL ERROR: Server cannot start without database.";
        return -1;
    }

    NetworkServer netServer;
    netServer.startServer(8080);


    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
