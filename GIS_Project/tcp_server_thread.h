#ifndef TCP_SERVER_THREAD_H
#define TCP_SERVER_THREAD_H

#include <QThread>
#include <QtNetwork>
#include <QTcpSocket>
#include <container_list.h>
class Tcp_Server_Thread : public QThread
{
    Q_OBJECT
public:
    Tcp_Server_Thread(int socketDescriptor,QObject *parent=0);
    void run();//线程开启
    void Get_SprData(Container_List &Container_out);//获取需要分发的数据
signals:
    void error(QTcpSocket::SocketError socketError);
private:
    int socketDescriptor;
    Container_List *Container;
    QTcpSocket *tcpSocket;
private slots:
    void Get_Data();
    void showError(QAbstractSocket::SocketError);
};

#endif // TCP_SERVER_THREAD_H
