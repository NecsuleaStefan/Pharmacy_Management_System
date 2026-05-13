#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>

// 1. Constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Initialize network client
    // client = new NetworkClient(this);
    // Uncomment the line above once your NetworkClient is fully implemented!
}

// 2. Destructor
MainWindow::~MainWindow() {
    delete ui;
}

// 3. Button Click Logic
void MainWindow::on_btnConfirmOrder_clicked() {
    // Logic for confirming orders goes here
    ui->statusbar->showMessage("Buton Confirmare apăsat");
}

void MainWindow::on_btnUpdateStock_clicked() {
    // Logic for updating stock goes here
}

void MainWindow::on_btnRefresh_clicked() {
    // Logic for refreshing the table goes here
}

// 4. Server Response Handlers
void MainWindow::refreshOrderTable(const QJsonArray &orders) {
    // Logic to update the UI table goes here
}

void MainWindow::handleStockAlert(const QString &alertMessage) {
    QMessageBox::warning(this, "Alertă Stoc Scăzut", alertMessage);
}

void MainWindow::showSuccessMessage(const QString &message) {
    QMessageBox::information(this, "Succes", message);
}

void MainWindow::showErrorMessage(const QString &message) {
    QMessageBox::critical(this, "Eroare", message);
}