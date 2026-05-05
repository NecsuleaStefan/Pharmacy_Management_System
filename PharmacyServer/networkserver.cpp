#include "networkserver.h"
#include <QJsonDocument> // Add this
#include <QJsonObject>   // Add this
#include <QJsonParseError> // Add this

NetworkServer::NetworkServer(QObject *parent) : QObject{parent} {

    server = new QTcpServer(this);

    connect(server, &QTcpServer::newConnection, this, &NetworkServer::onNewConnection);


}

NetworkServer::~NetworkServer()
{
    server->close();
}

void NetworkServer::startServer(quint16 port)
{
    // Tell the server to listen to ANY IP address on the specified port
    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "[NET ERROR] Server failed to start:" << server->errorString();
    } else {
        qDebug() << "[NET INFO] Server is officially listening on port" << port;
    }
}
void NetworkServer::onNewConnection()
{
    QTcpSocket *clientSocket = server->nextPendingConnection();

    clients.append(clientSocket);
    qDebug() << "[NET INFO] New client connected! Total active clients:" << clients.size();

    connect(clientSocket, &QTcpSocket::readyRead, this, &NetworkServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &NetworkServer::onClientDisconnected);
}

void NetworkServer::onReadyRead()
{
    // Figure out exactly WHICH client sent the message
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    // Read data
    QByteArray incomingData = clientSocket->readAll();
    qDebug() << "\n[NET RX] Raw data received:" << incomingData;

    // 1. Try to parse the incoming bytes into a JSON document
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(incomingData, &parseError);

    //Is it valid JSON?
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "[NET ERROR] Invalid JSON received:" << parseError.errorString();
        clientSocket->write("{\"status\":\"error\", \"message\":\"Invalid format\"}");
        return;
    }

    // 3. Extract the object and check the "action"
    QJsonObject requestObj = jsonDoc.object();
    QString action = requestObj["action"].toString();

    qDebug() << "[NET INFO] Client requested action:" << action;

    // 4. Route the request based on the action
    if (action == "login_attempt")
    {
        QString user = requestObj["username"].toString();
        QString pass = requestObj["password"].toString();

        qDebug() << "[NET SEC] Attempting login for user:" << user;

        // (Later, we will check these against the SQLite Database!)
        // For now, just send a structured JSON success response:
        QJsonObject response;
        response["action"] = "login_response";
        response["status"] = "success";
        response["message"] = "Welcome, Admin!";

        clientSocket->write(QJsonDocument(response).toJson(QJsonDocument::Compact));
    }
    else
    {
        qDebug() << "[NET WARNING] Unknown action received:" << action;
    }

}

void NetworkServer::onClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    clients.removeAll(clientSocket);
    clientSocket->deleteLater();

    qDebug() << "[NET INFO] A client disconnected. Total active clients:" << clients.size();
}