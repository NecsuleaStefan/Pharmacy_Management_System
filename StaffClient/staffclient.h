#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include "../PharmacyServer/databasemanager.h"
class StaffClient : public QMainWindow {
    Q_OBJECT

public:
    explicit StaffClient(int staffId, QWidget *parent = nullptr);
    ~StaffClient();

private slots:
    // Core Functional Requirements
    void onConfirmOrderPressed();      // REQ-56: Confirm offline-paid orders [cite: 552]
    void onHandleRequestPressed();     // REQ-62: Respond to medicine requests [cite: 565]
    void onUpdateStockStatus();        // REQ-32: Update stock after deliveries [cite: 507]
    void onLogoutPressed();            // REQ-08: Log out button [cite: 460]

private:
    int m_staffId;
    DatabaseManager *dbManager;
    QString m_sessionToken;
    // UI Components for "Data-Rich" interface [cite: 432]
    QTableWidget *orderTable;          // View pending customer orders [cite: 549]
    QTableWidget *requestTable;        // View customer requests [cite: 563]
    QLabel *stockAlertLabel;           // REQ-22: Automatic threshold notifications [cite: 489]

    void setupUi();
    void checkStockThresholds();       // Logic for stock tracking [cite: 489]
};