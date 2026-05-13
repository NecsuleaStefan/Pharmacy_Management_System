#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "networkclient.h" // Same logic as Admin

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // REQ-56 & REQ-58: Confirmation and Stock logic
    void on_btnConfirmOrder_clicked();

    // REQ-32: Update stock after delivery
    void on_btnUpdateStock_clicked();

    // UI Refreshers (Mirroring Admin pattern)
    void on_btnRefresh_clicked();
    void refreshOrderTable(const QJsonArray &orders);
    void handleStockAlert(const QString &alertMessage);

    // Messages from Server
    void showSuccessMessage(const QString &message);
    void showErrorMessage(const QString &message);

private:
    Ui::MainWindow *ui;
    NetworkClient *client; // We need this to talk to the server!
};

#endif // MAINWINDOW_H