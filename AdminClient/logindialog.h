#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "networkclient.h" // Avem nevoie de el pentru a trimite cererea

namespace Ui { class LoginDialog; }

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_btnLogin_clicked();
    void handleLoginSuccess(const QString &role);
    void handleLoginFailed(const QString &msg);

private:
    Ui::LoginDialog *ui;
    NetworkClient *client; // Obiectul care vorbește cu serverul
};

#endif