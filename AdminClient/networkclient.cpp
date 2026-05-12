#include "networkclient.h"
#include "qjsonarray.h"
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QTcpSocket>

NetworkClient::NetworkClient(QObject *parent) : QObject{parent}
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::readyRead, this, &NetworkClient::onReadyRead);

    // 2. Just for debugging, let's print when we successfully connect
    connect(socket, &QTcpSocket::connected, this, []() {
        qDebug() << "[CLIENT INFO] Successfully connected to the server!";
    });

}

void NetworkClient::connectToServer(const QString &ip, quint16 port)
{
    qDebug() << "[CLIENT INFO] Attempting to connect to" << ip << "on port" << port;

    socket->connectToHost(ip, port);
}

void NetworkClient::sendMessage(const QString &message)
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "[CLIENT TX] Sending:" << message;
        socket->write(message.toUtf8()); // Converts text to network bytes
    }
    else
    {
        qDebug() << "[CLIENT ERROR] Cannot send message. Not connected to server.";
    }
}

void NetworkClient::onReadyRead()
{
    QByteArray data = socket->readAll();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "[CLIENT ERROR] Failed to parse server response:" << parseError.errorString();
        return;
    }

    QJsonObject response = doc.object();
    QString action = response["action"].toString();
    QString status = response["status"].toString();
    QString message = response["message"].toString();

    qDebug() << "[CLIENT RX] Action:" << action << "| Status:" << status;

    // Route the response to the correct Qt Signal
    if (action == "login_response") {
        if (status == "success") {
            emit loginSuccess(response["role"].toString());
        } else {
            emit loginFailed(message);
        }
    }
    else if (action == "employee_list") {
        emit employeeListReceived(response["employees"].toArray());
    }
    else {
        // If it's not a login or a list, it's a generic action (add/edit/delete)
        if (status == "success") {
            emit actionSuccess(message);
        } else if (status == "error") {
            emit actionFailed(message);
        }
    }
}

void NetworkClient::sendLoginRequest(const QString &username, const QString &password) {
    QJsonObject request;
    request["action"] = "login_attempt";
    request["username"] = username;
    request["password"] = password;
    sendMessage(QJsonDocument(request).toJson(QJsonDocument::Compact));
}

void NetworkClient::sendAddEmployeeRequest(const QString &name, const QString &role, const QString &email) {
    QJsonObject request;
    request["action"] = "add_employee";
    request["name"] = name;
    request["role"] = role;
    request["email"] = email;
    sendMessage(QJsonDocument(request).toJson(QJsonDocument::Compact));
}

void NetworkClient::sendGetAllEmployeesRequest() {
    QJsonObject request;
    request["action"] = "get_all_employees";
    sendMessage(QJsonDocument(request).toJson(QJsonDocument::Compact));
}

void NetworkClient::sendDeleteEmployeeRequest(const QString &name) {
    QJsonObject request;
    request["action"] = "delete_employee";
    request["name"] = name;
    sendMessage(QJsonDocument(request).toJson(QJsonDocument::Compact));
}

void NetworkClient::sendEditEmployeeRequest(const QString &oldName, const QString &newName, const QString &newRole) {
    QJsonObject request;
    request["action"] = "edit_employee";
    request["oldName"] = oldName;
    request["newName"] = newName;
    request["newRole"] = newRole;
    sendMessage(QJsonDocument(request).toJson(QJsonDocument::Compact));
}
