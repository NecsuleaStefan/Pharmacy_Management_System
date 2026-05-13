#include "requestrouter.h"
#include "qjsonarray.h"
#include <QJsonDocument>
#include <QDebug>

void RequestRouter::processRequest(const QJsonObject &requestObj, QTcpSocket *clientSocket, DatabaseManager *db)
{
    QString action = requestObj["action"].toString();
    qDebug() << "[ROUTER] Routing action:" << action;

    // The ONLY if/else block left in the entire server.
    // It does no work; it just delegates to the correct handler.
    if (action == "login_attempt") {
        handleLogin(requestObj, clientSocket, db);
    }
    else if (action == "add_employee") {
        handleAddEmployee(requestObj, clientSocket, db);
    }
    else if (action == "get_all_employees") {
        handleGetAllEmployees(clientSocket, db);
    }
    else if (action == "delete_employee") {
        handleDeleteEmployee(requestObj, clientSocket, db);
    }
    else if (action == "edit_employee") {
        handleEditEmployee(requestObj, clientSocket, db);
    }
    else if (action == "get_all_medicines") {
        handleGetAllMedicines(clientSocket, db);
    }
    else if (action == "add_medicine") {
        handleAddMedicine(requestObj, clientSocket, db);
    }
    else if (action == "delete_medicine") {
        handleDeleteMedicine(requestObj, clientSocket, db);
    }
    else if (action == "edit_medicine") {
        handleEditMedicine(requestObj, clientSocket, db);
    }
    else {
        qDebug() << "[ROUTER WARNING] Unknown action received:" << action;
        QJsonObject response;
        response["status"] = "error";
        response["message"] = "Unknown action.";
        sendResponse(clientSocket, response);
    }
}

// ==========================================
// SPECIFIC HANDLERS
// ==========================================

void RequestRouter::handleLogin(const QJsonObject &requestObj, QTcpSocket *clientSocket, DatabaseManager *db)
{
    QString user = requestObj["username"].toString();
    QString pass = requestObj["password"].toString();
    QString role;

    QJsonObject response;
    response["action"] = "login_response";


    /*if (db->authenticateEmployee(user, pass, role)) {
        response["status"] = "success";
        response["message"] = "Welcome, " + user + "!";
        response["role"] = role;
    } else {
        response["status"] = "error";
        response["message"] = "Invalid username or password.";
    } */
    sendResponse(clientSocket, response);
}

void RequestRouter::handleAddEmployee(const QJsonObject &requestObj, QTcpSocket *clientSocket, DatabaseManager *db)
{
    QString name = requestObj["name"].toString();
    QString role = requestObj["role"].toString();


    bool success = db->addEmployee(name.toLower().replace(" ", "_"), role, name, "Nou");

    QJsonObject response;
    response["action"] = "add_employee_response";
    response["status"] = success ? "success" : "error";
    response["message"] = success ? "Angajat salvat în baza de date!" : "Eroare la salvare în SQLite.";
    sendResponse(clientSocket, response);
}

void RequestRouter::handleGetAllEmployees(QTcpSocket *clientSocket, DatabaseManager *db)
{
    QJsonObject response;
    response["action"] = "employee_list";
    response["employees"] = db->getAllEmployeesFromDB();
    response["status"] = "success";
    sendResponse(clientSocket, response);
}

void RequestRouter::handleDeleteEmployee(const QJsonObject &requestObj, QTcpSocket *clientSocket, DatabaseManager *db)
{
    QString name = requestObj["name"].toString();
    bool success = db->deleteEmployee(name);

    QJsonObject response;
    response["status"] = success ? "success" : "error";
    response["message"] = success ? "Angajat șters!" : "Eroare la ștergere.";
    sendResponse(clientSocket, response);
}

void RequestRouter::handleEditEmployee(const QJsonObject &requestObj, QTcpSocket *clientSocket, DatabaseManager *db)
{
    QString oldName = requestObj["oldName"].toString();
    QString newName = requestObj["newName"].toString();
    QString newRole = requestObj["newRole"].toString();

    bool success = db->updateEmployee(oldName, newName, newRole);

    QJsonObject response;
    response["status"] = success ? "success" : "error";
    response["message"] = success ? "Datele angajatului au fost actualizate!" : "Eroare la update.";
    sendResponse(clientSocket, response);
}

// ==========================================
// HELPER METHOD
// ==========================================
void RequestRouter::sendResponse(QTcpSocket *clientSocket, const QJsonObject &response)
{
    // This entirely removes the need to write QJsonDocument(...).toJson() everywhere!
    QJsonDocument doc(response);
    clientSocket->write(doc.toJson(QJsonDocument::Compact));
}


void RequestRouter::handleGetAllMedicines(QTcpSocket *clientSocket, DatabaseManager *db) {
    QJsonObject response;
    response["action"] = "medicine_list_response";
    response["data"] = db->getAllMedicinesFromDB();
    response["status"] = "success";
    sendResponse(clientSocket, response); // Folosim helper-ul de capsulare
}

void RequestRouter::handleAddMedicine(const QJsonObject &requestObj, QTcpSocket *clientSocket, DatabaseManager *db) {
    QString name = requestObj["name"].toString();
    QString cat = requestObj["category"].toString();
    double price = requestObj["price"].toDouble();
    int qty = requestObj["quantity"].toInt();

    bool success = db->addMedicine(name, cat, price, qty);

    QJsonObject response;
    response["action"] = "add_medicine_response"; // Important pentru client
    response["status"] = success ? "success" : "error";
    response["message"] = success ? "Medicament adăugat!" : "Eroare la salvare.";
    sendResponse(clientSocket, response);
}


void RequestRouter::handleDeleteMedicine(const QJsonObject &requestObj, QTcpSocket *clientSocket, DatabaseManager *db) {
    QString name = requestObj["name"].toString();
    bool success = db->deleteMedicine(name);

    QJsonObject response;
    response["action"] = "delete_medicine_response";
    response["status"] = success ? "success" : "error";
    response["message"] = success ? "Medicament șters!" : "Eroare la ștergere.";
    sendResponse(clientSocket, response);
}

void RequestRouter::handleEditMedicine(const QJsonObject &requestObj, QTcpSocket *clientSocket, DatabaseManager *db) {
    QString oldName = requestObj["oldName"].toString();
    QString newName = requestObj["newName"].toString();
    QString cat = requestObj["category"].toString();
    double price = requestObj["price"].toDouble();
    int qty = requestObj["quantity"].toInt();

    bool success = db->updateMedicine(oldName, newName, cat, price, qty);

    QJsonObject response;
    response["action"] = "edit_medicine_response";
    response["status"] = success ? "success" : "error";
    response["message"] = success ? "Medicament actualizat!" : "Eroare la actualizare.";
    sendResponse(clientSocket, response);
}