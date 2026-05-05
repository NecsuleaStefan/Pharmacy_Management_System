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

};

#endif // DATABASEMANAGER_H
