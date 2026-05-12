#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox> // Adaugă asta pentru alerte faine
#include <QInputDialog>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new NetworkClient(this);

    // 1. ADAUGĂ ACEASTĂ LINIE AICI:
    connect(client, &NetworkClient::messageReceived, this, &MainWindow::handleServerResponse);

    client->connectToServer("127.0.0.1", 8080);

    QTimer::singleShot(100, this, [this]() {
        QJsonObject request;
        request["action"] = "login_attempt";
        request["username"] = "admin_user";
        request["password"] = "secret123";

        QJsonDocument doc(request);
        client->sendMessage(doc.toJson(QJsonDocument::Compact));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAddEmployee_clicked() {
    bool ok;
    // Cerem numele angajatului printr-un pop-up
    QString name = QInputDialog::getText(this, "Adăugare Angajat",
                                         "Nume angajat:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !name.isEmpty()) {
        QJsonObject employee;
        employee["action"] = "add_employee";
        employee["name"] = name;
        employee["role"] = "Pharmacist";
        employee["email"] = name.toLower().replace(" ", ".") + "@farmacie.ro";

        QJsonDocument doc(employee);
        client->sendMessage(doc.toJson(QJsonDocument::Compact));
        ui->statusbar->showMessage("Se trimite cererea pentru: " + name);
    }
}

void MainWindow::handleServerResponse(const QString &message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject obj = doc.object();

    if (obj["action"] == "employee_list") {
        QJsonArray employees = obj["employees"].toArray();
        ui->tableEmployees->setRowCount(0);
        ui->tableEmployees->setColumnCount(2);
        ui->tableEmployees->setHorizontalHeaderLabels({"Nume", "Rol"});

        for (int i = 0; i < employees.size(); ++i) {
            QJsonObject emp = employees[i].toObject();
            int row = ui->tableEmployees->rowCount();
            ui->tableEmployees->insertRow(row);

            ui->tableEmployees->setItem(row, 0, new QTableWidgetItem(emp["name"].toString()));
            ui->tableEmployees->setItem(row, 1, new QTableWidgetItem(emp["role"].toString()));
        }
        ui->statusbar->showMessage("Lista actualizată.");
    }
}

void MainWindow::on_btnRefresh_clicked() {
    QJsonObject request;
    request["action"] = "get_all_employees";

    QJsonDocument doc(request);
    client->sendMessage(doc.toJson(QJsonDocument::Compact));

    ui->statusbar->showMessage("Se descarcă lista de angajați...");
}


void MainWindow::on_btnDeleteEmployee_clicked() {
    int currentRow = ui->tableEmployees->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Atenție", "Te rog selectează un angajat din tabel!");
        return;
    }

    QString name = ui->tableEmployees->item(currentRow, 0)->text();

    if (QMessageBox::question(this, "Confirmare", "Sigur vrei să-l ștergi pe " + name + "?") == QMessageBox::Yes) {
        QJsonObject request;
        request["action"] = "delete_employee";
        request["name"] = name;

        client->sendMessage(QJsonDocument(request).toJson(QJsonDocument::Compact));
    }
}


void MainWindow::on_btnEditEmployee_clicked() {
    int currentRow = ui->tableEmployees->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Atenție", "Te rog selectează un angajat!");
        return;
    }

    QString oldName = ui->tableEmployees->item(currentRow, 0)->text();
    bool ok;
    QString newName = QInputDialog::getText(this, "Editează Angajat",
                                            "Noul nume pentru " + oldName + ":",
                                            QLineEdit::Normal, oldName, &ok);

    if (ok && !newName.isEmpty()) {
        QJsonObject request;
        request["action"] = "edit_employee";
        request["oldName"] = oldName;
        request["newName"] = newName;
        request["newRole"] = "Pharmacist";

        client->sendMessage(QJsonDocument(request).toJson(QJsonDocument::Compact));
    }
}