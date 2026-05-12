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
};

#endif // DATABASEMANAGER_H
