#include "tcpsocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>
//#include <struct_list.h>
//#include <mydatastream.h>
TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
    connect(this,&TcpSocket::readyRead,this,&TcpSocket::readData);
    dis = connect(this,&TcpSocket::disconnected,
        [&](){
            qDebug() << "disconnect" ;
            emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
            this->deleteLater();
        });
    connect(&watcher,&QFutureWatcher<QByteArray>::finished,this,&TcpSocket::startNext);
    connect(&watcher,&QFutureWatcher<QByteArray>::canceled,this,&TcpSocket::startNext);
    qDebug() << "new connect" ;
}

TcpSocket::~TcpSocket()
{
}


void TcpSocket::sentData(const QByteArray &data, const int id)
{
    if(id == socketID)
    {
        write(data);
    }
}

void TcpSocket::disConTcp(int i)
{
    if (i == socketID)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        this->deleteLater();
    }
}

void TcpSocket::readData()
{
    this->readAll();
    qDebug()<<Container->Points_List.at(2).Point.x();
    QByteArray str1="test2";
    QByteArray str2=" of myew";
    datas.append(str1);
    datas.append(str2);
//    auto data  = handleData(this->readAll(),this->peerAddress().toString(),this->peerPort());
//    auto test =this->readAll();
//    qDebug() << data;
//    this->write(test);
//    this->write(data);
    if (!watcher.isRunning())//放到异步线程中处理。--这个特么好屌
    {
        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue()));
    }
}

QByteArray TcpSocket::handleData(QByteArray data)
{
    QTime time;
    time.start();

    QElapsedTimer tm;
    tm.start();
    while(tm.elapsed() < 100)
    {}
    return data;
}

void TcpSocket::startNext()
{
    this->write(watcher.future().result());
    if (!datas.isEmpty())
    {
        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue()));
    }
}
void TcpSocket::Get_Container(Container_List &Container_out){
    Container=&Container_out;
}
