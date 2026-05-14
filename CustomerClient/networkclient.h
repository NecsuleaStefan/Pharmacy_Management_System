#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

class NetworkClient : public QObject
{
    Q_OBJECT
public:
    explicit NetworkClient(QObject *parent = nullptr);

    void connectToServer(const QString &ip, quint16 port);

    void sendRegisterRequest(const QString &username, const QString &password, const QString &email,
                             const QString &phone, const QString &address);

    void sendLoginRequest(const QString &username, const QString &password);

    void sendGetAllMedicinesRequest();

signals:
    void registrationSuccess(const QString &message);
    void registrationFailed(const QString &errorMessage);

    void loginSuccess(const QString &message);
    void loginFailed(const QString &errorMessage);

    void medicineListReceived(const QJsonArray &medicines);

private slots:
    void onReadyRead();

private:
    QTcpSocket *m_socket;
    void sendMessage(const QJsonObject &json);
};
#endif // NETWORKCLIENT_H