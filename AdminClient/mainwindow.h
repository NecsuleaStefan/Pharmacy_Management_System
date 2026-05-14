#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "networkclient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_btnAddEmployee_clicked();
    void on_btnRefresh_clicked();
    void on_btnDeleteEmployee_clicked();
    void on_btnEditEmployee_clicked();
    void on_btnRefreshInventory_clicked();
    void on_btnAddMedicine_clicked();
    void on_btnDeleteMedicine_clicked();
    void on_btnEditMedicine_clicked();

    void refreshEmployeeTable(const QJsonArray &employees);
    void refreshMedicineTable(const QJsonArray &medicines);

    void showSuccessMessage(const QString &message);
    void showErrorMessage(const QString &message);

private:
    Ui::MainWindow *ui;
    NetworkClient *client;
};

#endif // MAINWINDOW_H
