#include "tcp_server.h"
#include "tcp_server_thread.h"
Tcp_Server::Tcp_Server(QObject *parent)
    :QTcpServer(parent)
{
}

void Tcp_Server::incomingConnection(int socketDescriptor)
{
    Tcp_Server_Thread *thread = new Tcp_Server_Thread(socketDescriptor,0);
    thread->Get_SprData(*Container);//把服务器端的容器放到新的处理线程中
    thread->start();//线程开始运行
}

void Tcp_Server::Get_SqrData(Container_List &Container_out){
    this->Container=&Container_out;
}
