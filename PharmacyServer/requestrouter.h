#ifndef REQUESTROUTER_H
#define REQUESTROUTER_H

#include <QJsonObject>
#include <QTcpSocket>
#include "databasemanager.h"

class RequestRouter
{
public:
    // The main entry point that the NetworkServer will call
    static void processRequest(const QJsonObject &requestObj, QTcpSocket *clientSocket, DatabaseManager *db);

private:
    // --------------------------------------------------------
    // Dedicated Handler Functions (One for each specific action)
    // --------------------------------------------------------
    static void handleLogin(const QJsonObject &requestObj, QTcpSocket *clientSocket, DatabaseManager *db);
    static void handleAddEmployee(const QJsonObject &requestObj, QTcpSocket *clientSocket, DatabaseManager *db);
    static void handleGetAllEmployees(QTcpSocket *clientSocket, DatabaseManager *db);
    static void handleDeleteEmployee(const QJsonObject &requestObj, QTcpSocket *clientSocket, DatabaseManager *db);
    static void handleEditEmployee(const QJsonObject &requestObj, QTcpSocket *clientSocket, DatabaseManager *db);

    // --------------------------------------------------------
    // Utility function to stop repeating QJsonDocument code
    // --------------------------------------------------------
    static void sendResponse(QTcpSocket *clientSocket, const QJsonObject &response);
};

#endif // REQUESTROUTER_H