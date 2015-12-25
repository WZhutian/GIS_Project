#include "database.h"
#include <QMessageBox>
Database::Database()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(QString("DRIVER={SQL SERVER};"
                               "SERVER=%1;"
                               "DATABASE=%2;"
                               "UID=%3;"
                               "PWD=%4;")
                       .arg("(local)")
                       .arg("GIS_Project_DB")
                       .arg("sa")
                       .arg("123456"));
    if (!db.open())
    {

        QMessageBox::warning(0, qApp->tr("Cannot open database"),
                             db.lastError().databaseText(), QMessageBox::Cancel);
    }
    else
    {
        qDebug()<<"Connect to Database Success!";
    }
}

void Database::Get_Container(Container_List &Container_Out){
    Container=&Container_Out;
}

void Database::Serch_Info(){

}

void Database::Add_Info(){

}

void Database::Delete_Info(){

}
