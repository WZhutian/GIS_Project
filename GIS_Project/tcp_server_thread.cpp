#include "tcp_server_thread.h"
#include <container_list.h>
#include <gdal_readfile.h>
#include <mydatastream.h>
#include <struct_list.h>
Tcp_Server_Thread::Tcp_Server_Thread(int socketDescriptor,QObject *parent)
    :QThread(parent),socketDescriptor(socketDescriptor)
{
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(Get_Data()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(showError(QAbstractSocket::SocketError)));

}
void Tcp_Server_Thread::Get_SprData(Container_List &Container_out){
    this->Container=&Container_out;
}

void Tcp_Server_Thread::showError(QAbstractSocket::SocketError)
{
    qDebug() << "tcpSocket->errorString()"; //输出错误信息
    qDebug() << tcpSocket->errorString(); //输出错误信息
}
void Tcp_Server_Thread::run()
{
    if(!tcpSocket->setSocketDescriptor(socketDescriptor))
    {
        emit error(tcpSocket->error());
        return;
    }
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
//    Container_List Container;
//    GDAL_ReadFile test("D:\\pnt1.shp",Container);
//    test.Get_Data();
    out<<Container->Points_List.at(9).Point;
    tcpSocket->write(block);
    tcpSocket->disconnectFromHost();
    tcpSocket->waitForDisconnected();
}
void Tcp_Server_Thread::Get_Data(){//获取服务器发来的矢量化数据
    QDataStream in(tcpSocket);
    St_Points mes;
    in >> mes;
    //将接收到的数据存放到变量中
    qDebug()<<mes.Point.x();
    //显示接收到的数据
}
