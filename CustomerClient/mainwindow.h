#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <networkclient.h>

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

private:
    Ui::MainWindow *ui;
    NetworkClient *client;

private slots:
    void populateStorefront(const QJsonArray &medicines);
};
#endif // MAINWINDOW_H
