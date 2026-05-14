#include "databasemanager.h"
#include <QJsonObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QCryptographicHash>
#include <QJsonArray>
#include <QJsonObject>

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

bool DatabaseManager::addEmployee(const QString &username, const QString &role, const QString &firstName, const QString &lastName) {
    QSqlQuery query;
    query.prepare("INSERT INTO Employee (username, password, first_name, last_name, role) "
                  "VALUES (:user, :pass, :fname, :lname, :role)");
    query.bindValue(":user", username);
    query.bindValue(":pass", "parola_default"); // Ulterior folosim hashing (REQ-284)
    query.bindValue(":fname", firstName);
    query.bindValue(":lname", lastName);
    query.bindValue(":role", role);

    if (!query.exec()) {
        qDebug() << "[DB ERROR] Failed to add employee:" << query.lastError().text();
        return false;
    }
    return true;
}

QJsonArray DatabaseManager::getAllEmployeesFromDB() {
    QJsonArray list;
    QSqlQuery query("SELECT first_name, role FROM Employee");

    while (query.next()) {
        QJsonObject emp;
        emp["name"] = query.value(0).toString();
        emp["role"] = query.value(1).toString();
        list.append(emp);
    }
    return list;
}

bool DatabaseManager::deleteEmployee(const QString &name) {
    QSqlQuery query;
    query.prepare("DELETE FROM Employee WHERE first_name = :name");
    query.bindValue(":name", name);
    return query.exec();
}

bool DatabaseManager::updateEmployee(const QString &oldName, const QString &newName, const QString &newRole) {
    QSqlQuery query;
    query.prepare("UPDATE Employee SET first_name = :newName, role = :newRole WHERE first_name = :oldName");
    query.bindValue(":newName", newName);
    query.bindValue(":newRole", newRole);
    query.bindValue(":oldName", oldName);

    if (!query.exec()) {
        qDebug() << "[DB ERROR] Failed to update employee:" << query.lastError().text();
        return false;
    }
    return true;
}
bool DatabaseManager::addCustomer(const QString &username, const QString &password,
                                  const QString &email, const QString &phone, const QString &address)
{
    QSqlQuery query;

    //hashing password
    QString hashedPass = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    query.prepare("INSERT INTO Customer (username, password, email, phone, address) "
                  "VALUES (:user, :pass, :email, :phone, :address)");
    query.bindValue(":user", username);
    query.bindValue(":pass", hashedPass);
    query.bindValue(":email", email);
    query.bindValue(":phone", phone);
    query.bindValue(":address", address);

    if (!query.exec()) {
        qDebug() << "[DB ERROR] Failed to register customer:" << query.lastError().text();
        return false;
    }
    return true;

}
bool DatabaseManager::authenticateCustomer(const QString &username, const QString &password) {
    // Scramble the password the user just typed in
    QString hashedPass = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    QSqlQuery query;
    // Check if a row exists with this exact username AND the exact scrambled password
    query.prepare("SELECT user_id FROM Customer WHERE username = :user AND password = :pass");
    query.bindValue(":user", username);
    query.bindValue(":pass", hashedPass);

    if (query.exec() && query.next()) {
        return true; // found!
    }
    return false; // Wrong username or password
}

QJsonArray DatabaseManager::getAllMedicinesFromDB() {
    QJsonArray list;
    QSqlQuery query("SELECT name, category, price, stk_quantity FROM Medicine");
    while (query.next()) {
        QJsonObject med;
        med["name"] = query.value(0).toString();
        med["category"] = query.value(1).toString();
        med["price"] = query.value(2).toDouble();
        med["quantity"] = query.value(3).toInt();
        list.append(med);
    }
    return list;
}

bool DatabaseManager::addMedicine(const QString &name, const QString &category, double price, int quantity) {
    QSqlQuery query;
    query.prepare("INSERT INTO Medicine (name, category, price, stk_quantity) "
                  "VALUES (:name, :cat, :price, :qty)");
    query.bindValue(":name", name);
    query.bindValue(":cat", category);
    query.bindValue(":price", price);
    query.bindValue(":qty", quantity);
    return query.exec();
}


bool DatabaseManager::deleteMedicine(const QString &name) {
    QSqlQuery query;
    query.prepare("DELETE FROM Medicine WHERE name = :name");
    query.bindValue(":name", name);
    return query.exec();
}


bool DatabaseManager::updateMedicine(const QString &oldName, const QString &newName, const QString &newCat, double newPrice, int newQty){
    QSqlQuery query;
    query.prepare("UPDATE Medicine SET name = :newName, category = :newCat, price = :newPrice, stk_quantity = :newQty WHERE name = :oldName");
    query.bindValue(":newName", newName);
    query.bindValue(":newCat", newCat);
    query.bindValue(":newPrice", newPrice);
    query.bindValue(":newQty", newQty);
    query.bindValue(":oldName", oldName);
    return query.exec();
}