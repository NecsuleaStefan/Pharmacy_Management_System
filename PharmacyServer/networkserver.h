#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QObject>

class NetworkServer : public QObject
{
    Q_OBJECT;

public:
    explicit NetworkServer(QObject *parent = nullptr);

    ~NetworkServer();

    void startServer(quint16 port);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    QTcpServer *server;

    QList<QTcpSocket*> clients;
};

#endif // NETWORKSERVER_H
