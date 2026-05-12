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

    // Auto-refresh the table so the user immediately sees the new/edited/deleted employee!
    client->sendGetAllEmployeesRequest();
}

void MainWindow::showErrorMessage(const QString &message) {
    ui->statusbar->showMessage("Eroare: " + message);
    QMessageBox::warning(this, "Eroare", message);
}