#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QSqlDatabase>
class DatabaseManager
{
private:

    QSqlDatabase db;
    bool createUsersTables();
    bool createInventoryTables();
    bool createCustomerOperationsTables();
    bool createSupplierOperationsTables();

public:
    DatabaseManager();
    bool initializeDatabase();
    QJsonArray getAllEmployeesFromDB();
    bool addEmployee(const QString &username, const QString &role, const QString &firstName, const QString &lastName);
    bool deleteEmployee(const QString &name);
    bool updateEmployee(const QString &oldName, const QString &newName, const QString &newRole);
    bool confirmOfflineOrder(int orderId, int staffId); // REQ-56
    bool allocateStockForOrder(int orderId);            // REQ-58
    bool handleMedicineRequest(int requestId, const QString &status); // REQ-64
    void logAction(int userId, const QString &action);  // REQ-68, REQ-69
};

#endif // DATABASEMANAGER_H
