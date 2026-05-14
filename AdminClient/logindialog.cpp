#include "logindialog.h"
#include "ui_logindialog.h"
#include "mainwindow.h" // Fereastra ta de Admin

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);

    client = new NetworkClient(this);
    client->connectToServer("127.0.0.1", 8080); // Te conectezi la serverul pornit de colegul tău

    // Conectăm răspunsurile serverului la logica noastră
    connect(client, &NetworkClient::loginSuccess, this, &LoginDialog::handleLoginSuccess);
    connect(client, &NetworkClient::loginFailed, this, &LoginDialog::handleLoginFailed);
}

void LoginDialog::on_btnLogin_clicked() {
    QString user = ui->editUsername->text();
    QString pass = ui->editPassword->text();

    if(user.isEmpty() || pass.isEmpty()) {
        ui->lblError->setText("Introdu user și parolă!");
        return;
    }

    client->sendLoginRequest(user, pass);
}

void LoginDialog::handleLoginSuccess(const QString &role) {
    if (role == "admin") {
        MainWindow *mainWin = new MainWindow();
        mainWin->show();
        this->accept(); // Închide login-ul cu succes
    } else {
        ui->lblError->setText("Acces neautorizat pentru rolul: " + role);
    }
}

void LoginDialog::handleLoginFailed(const QString &msg) {
    ui->lblError->setText(msg);
}

LoginDialog::~LoginDialog() { delete ui; }