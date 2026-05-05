#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
DatabaseManager::DatabaseManager() {


    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("pharmacy_system.db");

}
bool DatabaseManager::initializeDatabase()
{
    if (!db.open()) {
        qDebug() << "[DB ERROR] Could not connect to database:" << db.lastError().text();
        return false;
    }
    qDebug() << "[DB INFO] Connected to SQLite database at:" << QDir::currentPath() + "/pharmacy_system.db";
    if (!createUsersTables() ||
        !createInventoryTables() ||
        !createCustomerOperationsTables() ||
        !createSupplierOperationsTables()) {
        return false;
    }

    qDebug() << "[DB INFO] All database tables verified and ready.";
    return true;
}
bool DatabaseManager::createUsersTables()
{
    QSqlQuery query;

    QString customerSql = R"(
        CREATE TABLE IF NOT EXISTS Customer (
            user_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            email TEXT UNIQUE,
            phone TEXT,
            address TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";

    QString employeeSql = R"(
        CREATE TABLE IF NOT EXISTS Employee (
            employee_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            first_name TEXT NOT NULL,
            last_name TEXT NOT NULL,
            role TEXT NOT NULL,
            status TEXT DEFAULT 'Active'
        );
    )";

    if (!query.exec(customerSql)) {
        qDebug() << "[DB ERROR] Failed to create Customer table:" << query.lastError().text();
        return false;
    }
    if (!query.exec(employeeSql)) {
        qDebug() << "[DB ERROR] Failed to create Employee table:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::createInventoryTables()
{
    QSqlQuery query;

    QString medicineSql = R"(
        CREATE TABLE IF NOT EXISTS Medicine (
            medicine_id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            category TEXT,
            description TEXT,
            manufacturer TEXT,
            price REAL NOT NULL,
            stk_quantity INTEGER DEFAULT 0,
            expiration_date DATE
        );
    )";

    if (!query.exec(medicineSql)) {
        qDebug() << "[DB ERROR] Failed to create Medicine table:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::createCustomerOperationsTables()
{
    QSqlQuery query;

    QString feedbackSql = R"(
        CREATE TABLE IF NOT EXISTS Feedback (
            feedback_id INTEGER PRIMARY KEY AUTOINCREMENT,
            customer_id INTEGER,
            content TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(customer_id) REFERENCES Customer(user_id)
        );
    )";

    QString itemRequestSql = R"(
        CREATE TABLE IF NOT EXISTS Item_Request (
            request_id INTEGER PRIMARY KEY AUTOINCREMENT,
            customer_id INTEGER,
            item_name TEXT NOT NULL,
            status TEXT DEFAULT 'Pending',
            FOREIGN KEY(customer_id) REFERENCES Customer(user_id)
        );
    )";

    QString customerOrderSql = R"(
        CREATE TABLE IF NOT EXISTS Customer_Order (
            order_id INTEGER PRIMARY KEY AUTOINCREMENT,
            customer_id INTEGER,
            employee_id INTEGER,
            status TEXT DEFAULT 'Pending',
            total_price REAL DEFAULT 0.0,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(customer_id) REFERENCES Customer(user_id),
            FOREIGN KEY(employee_id) REFERENCES Employee(employee_id)
        );
    )";

    QString orderContainsSql = R"(
        CREATE TABLE IF NOT EXISTS Order_Contains (
            order_id INTEGER,
            medicine_id INTEGER,
            quantity INTEGER NOT NULL,
            PRIMARY KEY (order_id, medicine_id),
            FOREIGN KEY(order_id) REFERENCES Customer_Order(order_id),
            FOREIGN KEY(medicine_id) REFERENCES Medicine(medicine_id)
        );
    )";

    if (!query.exec(feedbackSql) || !query.exec(itemRequestSql) ||
        !query.exec(customerOrderSql) || !query.exec(orderContainsSql)) {
        qDebug() << "[DB ERROR] Failed to create Customer Operations tables:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::createSupplierOperationsTables()
{
    QSqlQuery query;

    QString supplierSql = R"(
        CREATE TABLE IF NOT EXISTS Supplier (
            supplier_id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            contact_email TEXT,
            phone TEXT
        );
    )";

    QString supplierOrderSql = R"(
        CREATE TABLE IF NOT EXISTS Supplier_Order (
            order_id INTEGER PRIMARY KEY AUTOINCREMENT,
            supplier_id INTEGER,
            employee_id INTEGER,
            status TEXT DEFAULT 'Ordered',
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(supplier_id) REFERENCES Supplier(supplier_id),
            FOREIGN KEY(employee_id) REFERENCES Employee(employee_id)
        );
    )";

    QString supplierOrderContainsSql = R"(
        CREATE TABLE IF NOT EXISTS Supplier_Order_Contains (
            order_id INTEGER,
            medicine_id INTEGER,
            quantity INTEGER NOT NULL,
            PRIMARY KEY (order_id, medicine_id),
            FOREIGN KEY(order_id) REFERENCES Supplier_Order(order_id),
            FOREIGN KEY(medicine_id) REFERENCES Medicine(medicine_id)
        );
    )";

    if (!query.exec(supplierSql) || !query.exec(supplierOrderSql) || !query.exec(supplierOrderContainsSql)) {
        qDebug() << "[DB ERROR] Failed to create Supplier Operations tables:" << query.lastError().text();
        return false;
    }
    return true;
}