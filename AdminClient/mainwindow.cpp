#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new NetworkClient(this);
    client->connectToServer("127.0.0.1", 8080);

    // (We use a tiny timer to give the socket 100 milliseconds to finish connecting)
    QTimer::singleShot(100, this, [this]() {

        // 1. Create a JSON Object
        QJsonObject request;
        request["action"] = "login_attempt";
        request["username"] = "admin_user";
        request["password"] = "secret123";

        // 2. Convert it into network bytes (UTF-8 String)
        QJsonDocument doc(request);
        QString jsonString = doc.toJson(QJsonDocument::Compact);
        client->sendMessage(jsonString);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
