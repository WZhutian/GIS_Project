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
    Container.PC_ID=0;
    TcpServer ser;
    ser.Get_Container(Container);
    MainWindow w;
    w.Get_Container(Container);
    w.Get_TcpServer(ser);
    w.show();

    return a.exec();
}
