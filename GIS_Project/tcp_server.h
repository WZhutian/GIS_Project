#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <container_list.h>

#include <QTcpServer>
class Tcp_Server: public QTcpServer
{
    Q_OBJECT
public:
    Tcp_Server(QObject *parent=0);
    void Get_SqrData(Container_List &Container_out);
protected:
    void incomingConnection(int socketDescriptor);
private:
    Container_List *Container;
};

#endif // TCP_SERVER_H
