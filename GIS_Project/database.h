#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QtSql>
#include "container_list.h"
#include <QString>
#include <QStringList>
#include "editwidget.h"
class Database
{
public:
    Database();
    QSqlDatabase db;
    QStringList Get_Projcet();
    bool Add_Project(QString Name);
    void Connect();
    int Get_Project_Size();
    bool Get_Info(int ID);
    bool Add_Info(int ID);
    void Delete_Info(int ID);
    int Get_Project_Last();
    Container_List *Container;
    EditWidget *area;
    void Get_Container(Container_List &Container_Out);
    void Get_Area(EditWidget &Area_out){
        area=&Area_out;
    }
};

#endif // DATABASE_H
