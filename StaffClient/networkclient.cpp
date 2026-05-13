#include "networkclient.h"
#include "qjsonarray.h"
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QTcpSocket>
NetworkClient::NetworkClient(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkClient::onReadyRead);
}

void NetworkClient::connectToServer(const QString &host, quint16 port) {
    socket->connectToHost(host, port);
}

void NetworkClient::sendRequest(const QJsonObject &request) {
    if (socket->state() == QTcpSocket::ConnectedState) {
        QJsonDocument doc(request);
        socket->write(doc.toJson(QJsonDocument::Compact) + "\n");
    }
}

void NetworkClient::sendConfirmOrderRequest(int orderId) {
    QJsonObject req;
    req["type"] = "CONFIRM_ORDER";
    req["orderId"] = orderId;
    sendRequest(req);
}

void NetworkClient::sendGetAllOrdersRequest() {
    QJsonObject req;
    req["type"] = "GET_ORDERS";
    sendRequest(req);
}

void NetworkClient::onReadyRead() {
    while (socket->canReadLine()) {
        QByteArray data = socket->readLine();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull() || !doc.isObject()) continue;

        QJsonObject response = doc.object();
        QString type = response["type"].toString();

        if (type == "ORDERS_LIST") {
            emit orderListReceived(response["orders"].toArray());
        } else if (type == "SUCCESS") {
            emit actionSuccess(response["message"].toString());
        } else if (type == "ERROR") {
            emit actionFailed(response["message"].toString());
        } else if (type == "STOCK_ALERT") {
            emit stockAlertReceived(response["message"].toString());
        }
    }
}