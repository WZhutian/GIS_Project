#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <gdal_readfile.h>
#include <tcp_server.h>
#include <tcp_client.h>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Container_List Container;
    GDAL_ReadFile test("D:\\pnt1.shp",Container);
    test.Get_Data();
    qDebug()<<Container.Points_List.length();
    qDebug()<<Container.Points_List.at(0).Point.x();
    Tcp_Server New_server;
    New_server.Get_SqrData(Container);
    if(!New_server.listen(QHostAddress::LocalHost,6666)){
        qDebug()<<"wrong";
    }
    qDebug()<<"blocked";
    MainWindow w;
    w.show();

    return a.exec();
}
