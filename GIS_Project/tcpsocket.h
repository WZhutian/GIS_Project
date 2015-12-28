﻿#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>


#include <container_list.h>
#include "editwidget.h"
#include <QMutex>
class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(qintptr socketDescriptor, QObject *parent = 0);
    ~TcpSocket();
    QByteArray handleData(QByteArray data);//用来处理数据的函数
    Container_List *Container;//保存当前容器
    void Get_Container(Container_List &Container_out);
    EditWidget *area;
    void Get_Area(EditWidget &area_out){
        area=&area_out;
    }
QMutex mutex;
    QByteArray  intToByte(int i);
    int bytesToInt(QByteArray bytes);
signals:
    //void readData(const int,const QString &,const quint16,const QByteArray &);
    void sockDisConnect(const int ,const QString &,const quint16, QThread *);//NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
public slots:
    void sentData(const QByteArray & ,const int);//发送信号的槽
    void disConTcp(int i);

protected slots:
    void readData();//接收数据
    void startNext();//处理下一个
protected:
    QFutureWatcher<QByteArray> watcher;
    QQueue<QByteArray> datas;
private:
    qintptr socketID;
    QMetaObject::Connection dis;
};

#endif // TCPSOCKET_H
