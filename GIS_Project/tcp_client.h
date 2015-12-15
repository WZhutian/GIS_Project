#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <mydatastream.h>
class Tcp_Client :public QObject
{
    Q_OBJECT
public:
    Tcp_Client(QString Address,int Port);
    void showError(QAbstractSocket::SocketError socketError);
public slots:
    void Get_Data();
private:
    QTcpSocket *tcpSocket;
    QString Address;//主机IP
    int Port;//主机端口
};

#endif // TCP_CLIENT_H
