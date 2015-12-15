#include "tcp_client.h"
#include <QDebug>
#include <QObject>
Tcp_Client::Tcp_Client(QString Address,int Port)
{
    this->Address=Address;
    this->Port=Port;
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(Get_Data()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(showError(QAbstractSocket::SocketError)));

}
void Tcp_Client::Get_Data(){

    tcpSocket->abort();
    tcpSocket->connectToHost(Address,Port);
    QDataStream in(tcpSocket);

    if(tcpSocket->bytesAvailable()<(int)sizeof(uint))
        return;
    St_Points test;
    in>>test;
    qDebug()<<test.Point.x();

}

void Tcp_Client::showError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug()<<"主机不可达！";
        break;
    case QAbstractSocket::ConnectionRefusedError:
         qDebug()<<"连接被拒绝！";
        break;
    default:
         qDebug()<<tcpSocket->errorString();
    }
}


