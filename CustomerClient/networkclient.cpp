#include "networkclient.h"
#include "qjsonarray.h"
#include <QJsonParseError>
#include <QDebug>

NetworkClient::NetworkClient(QObject *parent) : QObject(parent) {
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkClient::onReadyRead);

    // --- Server connection Debuggers ---

    // 1. Tell us if it successfully connects
    connect(m_socket, &QTcpSocket::connected, this, []() {
        qDebug() << "[CLIENT INFO] Customer Client successfully connected to the Server!";
    });

    // 2. Tell us if it can't connect
    connect(m_socket, &QTcpSocket::errorOccurred, this, []() {
        qDebug() << "[CLIENT ERROR] Customer Client Failed to connect to the Server";
    });
}

void NetworkClient::connectToServer(const QString &ip, quint16 port) {
    m_socket->connectToHost(ip, port);

}

void NetworkClient::sendMessage(const QJsonObject &json) {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(QJsonDocument(json).toJson(QJsonDocument::Compact));
    }
}

void NetworkClient::sendRegisterRequest(const QString &username, const QString &password, const QString &email, const QString &phone, const QString &address) {
    QJsonObject request;
    request["action"] = "register_customer";
    request["username"] = username;
    request["password"] = password;
    request["email"] = email;
    request["phone"] = phone;
    request["address"] = address;
    sendMessage(request);
}
void NetworkClient::sendLoginRequest(const QString &username, const QString &password) {
    QJsonObject request;
    request["action"] = "customer_login";
    request["username"] = username;
    request["password"] = password;
    sendMessage(request);
}

void NetworkClient::sendGetAllMedicinesRequest() {
    QJsonObject request;
    request["action"] = "get_all_medicines";
    sendMessage(request);
}

void NetworkClient::onReadyRead() {
    QByteArray data = m_socket->readAll();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError) return;

    QJsonObject response = doc.object();
    QString action = response["action"].toString();
    QString status = response["status"].toString();
    QString message = response["message"].toString();

    if (action == "register_response") {
        if (status == "success") emit registrationSuccess(message);
        else emit registrationFailed(message);
    }
    else if (action == "customer_login_response") {
        if (status == "success") emit loginSuccess(message);
        else emit loginFailed(message);
    }
    else if(action == "medicine_list_response")
    {
        emit medicineListReceived(response["data"].toArray());
    }
}