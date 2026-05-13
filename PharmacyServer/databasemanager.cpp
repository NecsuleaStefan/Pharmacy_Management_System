#include "databasemanager.h"
#include <QJsonObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
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

// ~~~~~~~~~~~~~~~~~~~~~[StaffClient]~~~~~~~~~~~~~~~~~~~~~

bool DatabaseManager::allocateStockForOrder(int orderId) {
    // REQ-58: Start a transaction to ensure data integrity
    if (!db.transaction()) {
        qDebug() << "[DB ERROR] Could not start transaction";
        return false;
    }

    QSqlQuery query;
    // Note: Using 'Order_Contains' to match your createCustomerOperationsTables() function
    query.prepare("SELECT medicine_id, quantity FROM Order_Contains WHERE order_id = ?");
    query.addBindValue(orderId);

    if (!query.exec()) {
        db.rollback();
        qDebug() << "[DB ERROR] Failed to fetch order items:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        int medId = query.value(0).toInt();
        int qty = query.value(1).toInt();

        // REQ-58: Update 'Medicine' table stock using 'stk_quantity' column
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE Medicine SET stk_quantity = stk_quantity - ? WHERE medicine_id = ?");
        updateQuery.addBindValue(qty);
        updateQuery.addBindValue(medId);

        if (!updateQuery.exec()) {
            db.rollback(); // Rollback if any single item fails to update
            qDebug() << "[DB ERROR] Stock update failed for Medicine ID:" << medId << " - " << updateQuery.lastError().text();
            return false;
        }
    }

    // REQ-56: Update the order status to 'Confirmed' or 'Completed'
    QSqlQuery statusQuery;
    statusQuery.prepare("UPDATE Customer_Order SET status = 'Confirmed' WHERE order_id = ?");
    statusQuery.addBindValue(orderId);

    if (!statusQuery.exec()) {
        db.rollback();
        return false;
    }

    qDebug() << "[DB INFO] Stock successfully allocated for Order ID:" << orderId;
    return db.commit(); // Finalize all changes
}