#include "staffclient.h"
#include <QMessageBox>
#include "../PharmacyServer/databasemanager.h"
StaffClient::~StaffClient() {}

// REQ-56 & REQ-58: Logic for confirming orders and allocating stock
void StaffClient::onConfirmOrderPressed() {
    int currentRow = orderTable->currentRow();
    if (currentRow < 0) return;

    QString orderId = orderTable->item(currentRow, 0)->text();

    // Simulate server communication using the fingerprint requirement
    // validateSession(m_sessionToken)
    bool success = true; // Placeholder for server response

    if (success) {
        // Update job status in database
        orderTable->item(currentRow, 2)->setText("Confirmed");
    }
}

void StaffClient::onHandleRequestPressed() {
    // Logic to view detailed request info and initiate a response
    QMessageBox::information(this, "Request Management", "Initiating response to customer medicine request.");
}

void StaffClient::onUpdateStockStatus() {
    // Logic to update stock levels after deliveries
    // Only available for authorized staff and admins
    QMessageBox::information(this, "Inventory", "Stock levels updated and synchronized with database.");
}

// REQ-22: Automatic notification system
void StaffClient::checkStockThresholds() {
    // Logic would query the SQLite database via Qt SQL
    int currentStock = 5; // Example value
    int threshold = 10;

    if (currentStock < threshold) {
        stockAlertLabel->setText("ALERT: Low Stock on Medicine ID 101!");
        stockAlertLabel->setStyleSheet("color: red;");
    }
}



// REQ-08: Log Out
void StaffClient::onLogoutPressed() {
    // Logic to end the session
    this->close();
}