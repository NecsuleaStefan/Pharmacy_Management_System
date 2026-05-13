#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class NetworkClient : public QObject {
    Q_OBJECT
public:
    explicit NetworkClient(QObject *parent = nullptr);
    void connectToServer(const QString &host, quint16 port);

    // --- STAFF REQUESTS ---
    void sendConfirmOrderRequest(int orderId);
    void sendGetAllOrdersRequest();

signals:
    // --- SIGNALS --- (This fixes the 'undeclared identifier' errors)
    void orderListReceived(const QJsonArray &orders);
    void stockAlertReceived(const QString &alertMessage);
    void actionSuccess(const QString &message);
    void actionFailed(const QString &message);

private slots:
    void onReadyRead();

private:
    QTcpSocket *socket;

    // --- HELPER FUNCTION --- (This fixes the sendRequest error)
    void sendRequest(const QJsonObject &request);
};

#endif // NETWORKCLIENT_H