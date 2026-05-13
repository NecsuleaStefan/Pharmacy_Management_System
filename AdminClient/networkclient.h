#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>

class NetworkClient : public QObject
{
    Q_OBJECT
public:
    explicit NetworkClient(QObject *parent = nullptr);

    void connectToServer(const QString &ip, quint16 port);
    void sendMessage(const QString &message);

    void sendLoginRequest(const QString &username, const QString &password);
    void sendAddEmployeeRequest(const QString &name, const QString &role, const QString &email);
    void sendGetAllEmployeesRequest();
    void sendDeleteEmployeeRequest(const QString &name);
    void sendEditEmployeeRequest(const QString &oldName, const QString &newName, const QString &newRole);

signals:
    void loginSuccess(const QString &role);
    void loginFailed(const QString &errorMessage);
    void employeeListReceived(const QJsonArray &employees);
    void actionSuccess(const QString &message);
    void actionFailed(const QString &errorMessage);

private slots:
void onReadyRead();

private:
QTcpSocket *socket;

};
#endif // NETWORKCLIENT_H
