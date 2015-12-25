#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QtSql>
#include "container_list.h"
class Database
{
public:
    Database();
    QSqlDatabase db;
    void Serch_Info();
    void Add_Info();
    void Delete_Info();
    Container_List *Container;
    void Get_Container(Container_List &Container_Out);
};

#endif // DATABASE_H
