#include "mainwindow.h"
#include <QApplication>

#include <gdal_readfile.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Container_List Container;
    GDAL_ReadFile test("D:\\pnt1.shp",Container);
    test.Get_Data();
    qDebug()<<Container.Points_List.length();
    qDebug()<<Container.Points_List.at(0).Point.x();
    qDebug()<<Container.Points_List.at(0).Point.y();
    MainWindow w;
    w.show();

    return a.exec();
}
