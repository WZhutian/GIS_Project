#ifndef TCPCLIENT_H
#define TCPCLIENT_H


#include <QtNetwork>
class TcpClient
{
public:
    TcpClient();
    ~TcpClient();
public slots:
    void ReadError(QAbstractSocket::SocketError);
private:
    QTcpSocket * tcpClient;
    QTimer tm;
    QStringList list;
};

#endif // TCPCLIENT_H
