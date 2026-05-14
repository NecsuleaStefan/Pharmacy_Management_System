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

    connect(client, &NetworkClient::employeeListReceived, this, &MainWindow::refreshEmployeeTable);
    connect(client, &NetworkClient::actionSuccess, this, &MainWindow::showSuccessMessage);
    connect(client, &NetworkClient::actionFailed, this, &MainWindow::showErrorMessage);
    connect(client, &NetworkClient::medicineListReceived, this, &MainWindow::refreshMedicineTable);

    client->connectToServer("127.0.0.1", 8080);


   /* QTimer::singleShot(100, this, [this]() {
        // client->sendLoginRequest("admin_user", "secret123");
    }); */
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
    if (ok && !name.isEmpty())
    {
        QString email = name.toLower().replace(" ", ".") + "@farmacie.ro";
        client->sendAddEmployeeRequest(name, "Pharmacist", email);
        ui->statusbar->showMessage("Se trimite cererea pentru: " + name);
    }
}


void MainWindow::on_btnRefresh_clicked() {
    client->sendGetAllEmployeesRequest();
    ui->statusbar->showMessage("Se descarcă lista de angajați...");
}


void MainWindow::on_btnDeleteEmployee_clicked() {
    int currentRow = ui->tableEmployees->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Atenție", "Te rog selectează un angajat din tabel!");
        return;
    }

    QString name = ui->tableEmployees->item(currentRow, 0)->text();

    if (QMessageBox::question(this, "Confirmare", "Sigur vrei să-l ștergi pe " + name + "?") == QMessageBox::Yes)
    {
        client->sendDeleteEmployeeRequest(name);
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

        client->sendEditEmployeeRequest(oldName, newName, "Pharmacist");
    }
}

void MainWindow::refreshEmployeeTable(const QJsonArray &employees) {
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



void MainWindow::showSuccessMessage(const QString &message) {
    ui->statusbar->showMessage(message);
    QMessageBox::information(this, "Succes", message);

    // Verificăm ce tab este activ și dăm refresh doar acolo
    if (ui->tabWidget->currentIndex() == 0) { // Tab Employees
        client->sendGetAllEmployeesRequest();
    } else if (ui->tabWidget->currentIndex() == 1) { // Tab Medicines
        client->sendGetAllMedicinesRequest();
    }
}

void MainWindow::showErrorMessage(const QString &message) {
    ui->statusbar->showMessage("Eroare: " + message);
    QMessageBox::warning(this, "Eroare", message);
}

void MainWindow::refreshMedicineTable(const QJsonArray &medicines) {
    ui->tableMedicine->setRowCount(0);
    ui->tableMedicine->setColumnCount(4);
    ui->tableMedicine->setHorizontalHeaderLabels({"Nume", "Categorie", "Preț", "Stoc"});
    for (int i = 0; i < medicines.size(); ++i) {
        QJsonObject med = medicines[i].toObject();
        int row = ui->tableMedicine->rowCount();
        ui->tableMedicine->insertRow(row);
        ui->tableMedicine->setItem(row, 0, new QTableWidgetItem(med["name"].toString()));
        ui->tableMedicine->setItem(row, 1, new QTableWidgetItem(med["category"].toString()));
        ui->tableMedicine->setItem(row, 2, new QTableWidgetItem(QString::number(med["price"].toDouble())));
        ui->tableMedicine->setItem(row, 3, new QTableWidgetItem(QString::number(med["quantity"].toInt())));
    }
}


void MainWindow::on_btnRefreshInventory_clicked() {
    client->sendGetAllMedicinesRequest();
}


void MainWindow::on_btnAddMedicine_clicked() {
    bool ok;
    QString name = QInputDialog::getText(this, "Add Medicine", "Nume:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    QString cat = QInputDialog::getText(this, "Add Medicine", "Categorie:", QLineEdit::Normal, "General", &ok);
    if (!ok) return;

    double price = QInputDialog::getDouble(this, "Add Medicine", "Preț:", 0.0, 0, 10000, 2, &ok);
    if (!ok) return;

    int qty = QInputDialog::getInt(this, "Add Medicine", "Stoc inițial:", 0, 0, 1000, 1, &ok);
    if (ok) {
        client->sendAddMedicineRequest(name, cat, price, qty);
    }
}


void MainWindow::on_btnDeleteMedicine_clicked() {
    int currentRow = ui->tableMedicine->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Atenție", "Selectează un medicament!");
        return;
    }

    QString name = ui->tableMedicine->item(currentRow, 0)->text();
    if (QMessageBox::question(this, "Confirmare", "Ștergi " + name + "?") == QMessageBox::Yes) {
        client->sendDeleteMedicineRequest(name);
    }
}

void MainWindow::on_btnEditMedicine_clicked() {
    int currentRow = ui->tableMedicine->currentRow();
    if (currentRow < 0) return;

    QString oldName = ui->tableMedicine->item(currentRow, 0)->text();
    QString oldCat = ui->tableMedicine->item(currentRow, 1)->text();
    double oldPrice = ui->tableMedicine->item(currentRow, 2)->text().toDouble();
    int oldQty = ui->tableMedicine->item(currentRow, 3)->text().toInt();

    bool ok;
    QString newName = QInputDialog::getText(this, "Edit", "Nume nou:", QLineEdit::Normal, oldName, &ok);
    if (!ok) return;

    double newPrice = QInputDialog::getDouble(this, "Edit", "Preț nou:", oldPrice, 0, 10000, 2, &ok);
    if (!ok) return;

    int newQty = QInputDialog::getInt(this, "Edit", "Stoc nou:", oldQty, 0, 1000, 1, &ok);
    if (ok) {
        client->sendEditMedicineRequest(oldName, newName, oldCat, newPrice, newQty);
    }
}