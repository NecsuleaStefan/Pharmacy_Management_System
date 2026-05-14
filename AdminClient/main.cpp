#include "mainwindow.h"
#include "logindialog.h" // Adăugăm header-ul nou
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Creăm și afișăm fereastra de Login
    LoginDialog login;

    // exec() blochează codul aici până când dialogul se închide
    // Dacă în LoginDialog am apelat accept(), exec() va returna QDialog::Accepted
    if (login.exec() == QDialog::Accepted) {
        // 2. Dacă am ajuns aici, înseamnă că login-ul a reușit!
        // MainWindow se va deschide automat din logica de succes a LoginDialog
        return a.exec();
    }

    // 3. Dacă user-ul a apăsat "X" sau login-ul a eșuat, aplicația se închide
    return 0;
}