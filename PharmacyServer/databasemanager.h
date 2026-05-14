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
    QJsonArray getAllMedicinesFromDB();

    bool addEmployee(const QString &username, const QString &role, const QString &firstName, const QString &lastName);
    bool deleteEmployee(const QString &name);
    bool updateEmployee(const QString &oldName, const QString &newName, const QString &newRole);

    bool addMedicine(const QString &name, const QString &category, double price, int quantity);
    bool deleteMedicine(const QString &name);
    bool updateMedicine(const QString &oldName, const QString &newName, const QString &newCat, double newPrice, int newQty);
};

#endif // DATABASEMANAGER_H
