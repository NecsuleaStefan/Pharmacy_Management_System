#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "networkclient.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = new NetworkClient(this);
    client->connectToServer("127.0.0.1", 8080);

    // app always open first on Page 0  of the stack(Login)
    ui->stackedWidget->setCurrentIndex(0);

    // user click "Register Instead" btn, flip to Page 1 of the stack (Register)
    connect(ui->btnGoToRegister, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    // user clicks "Back to login", flip to Page 0 of the stack (Login)
    connect(ui->btnBackToLogin, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    // send register request on click
    connect(ui->btnSubmitRegister, &QPushButton::clicked, this, [this]() {

        // Read the text boxes
        QString user = ui->RegUsername->text();
        QString pass = ui->RegPassword->text();
        QString email = ui->Email->text();
        QString phone = ui->Phone->text();
        QString address = ui->Address->text();

        client->sendRegisterRequest(user, pass, email, phone, address);

        //clear after submitting
         ui->RegUsername->clear();
         ui->RegPassword->clear();
         ui->Email->clear();
         ui->Phone->clear();
         ui->Address->clear();



    });

    //registration success
    connect(client, &NetworkClient::registrationSuccess, this, [this](const QString &msg) {
        // Show succes, then sends you to Login Page!
        qDebug() << "SUCCESS:" << msg;
        ui->stackedWidget->setCurrentIndex(0);
    });

    //registration failed
    connect(client, &NetworkClient::registrationFailed, this, [](const QString &err) {
        qDebug() << "FAILED:" << err;
    });

    //send log in request on click
    connect(ui->btnLogin, &QPushButton::clicked, this, [this]() {
        QString user = ui->LogUsername->text();
        QString pass = ui->LogPassword->text();
        client->sendLoginRequest(user, pass);
    });

    //log in success, sends to another page of the stakc
    connect(client, &NetworkClient::loginSuccess, this, [this](const QString &msg) {
        QMessageBox::information(this, "Success", msg);

        // Clear the password
        ui->LogPassword->clear();

        // Flip to Page 2 of the stack
        ui->stackedWidget->setCurrentIndex(2);
    });
    // 3. log in fails, show an error
    connect(client, &NetworkClient::loginFailed, this, [this](const QString &err) {
        QMessageBox::warning(this, "Login Failed", err);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
