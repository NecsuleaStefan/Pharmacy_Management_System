#include "networkclient.h"
#include <QObject>
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
    // Read the reply from the server
    QByteArray data = socket->readAll();
    QString reply = QString::fromUtf8(data);

    qDebug() << "[CLIENT RX] Server says:" << reply;

    // Broadcast this out to the rest of the app (like the UI)
    emit messageReceived(reply);
}
