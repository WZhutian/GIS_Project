#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <gdal_readfile.h>
#include <QDebug>
#include "tcpserver.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Container_List Container;
    GDAL_ReadFile test("D:\\pnt1.shp",Container);
    test.Get_Data();
    qDebug()<<Container.Points_List.length();
    qDebug()<<Container.Points_List.at(0).Point.x();
    TcpServer ser;
    ser.listen(QHostAddress::Any,6666);
    MainWindow w;
    w.show();

    return a.exec();
}
