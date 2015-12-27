#include "database.h"
#include <QMessageBox>
#include "mydatastream.h"
#include "QByteArray"
Database::Database()
{

}
void Database::Connect(){
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
QStringList Database::Get_Projcet(){
    QStringList return_temp;
    QSqlQuery query;
    query.exec("select * from Select_List");
    while (query.next())
    {
        return_temp<<(query.value(0).toString()+"."+query.value(1).toString());
    }
    return return_temp;
}

bool Database::Add_Project(QString Name){
    QSqlQuery query;
    bool success;
    query.prepare("insert into Select_List values(?)");
    query.bindValue(0,Name);
    success = query.exec();
    if(success){
        return true;
    }else{
        return false;
    }
}
int Database::Get_Project_Size(){
    QSqlQuery query;
    int num=0;
    query.exec("select * from Select_List");
    while(query.next()){
        num++;
    }
    return num;
}
int Database::Get_Project_Last(){
    QSqlQuery query;
    int id=-1;
    query.exec("select * from Select_List");
    while(query.next()){
        id=query.value(0).toInt();
    }
    return id+1;
}
bool Database::Get_Info(int ID){
    bool succeed_1;bool succeed_2;bool succeed_3;bool succeed_4;
    QSqlQuery query;
    succeed_1=query.exec(QString("select * from Layer_List where ID = %1").arg(ID));
    while (query.next())
    {
        St_Layers Layer;
        Layer.Layer_ID=query.value(0).toInt();
        Layer.Layer_Name = query.value(1).toString();
        Layer.Ob_Type=query.value(2).toInt();
        Layer.Size=query.value(3).toInt();
        Layer.Pc_numbers=query.value(4).toInt();
        QDataStream in(query.value(5).toByteArray());
        in>>Layer.Attribute_Name;
        QStringList temp_every;
        QDataStream in_2(query.value(6).toByteArray());
        in_2>>temp_every;
        Container->Layers_List.append(Layer);
        //TODO
        St_Items temp_items;
        Container->Items_List.append(temp_items);
    }
    succeed_2=query.exec(QString("select * from Point_List where ID = %1").arg(ID));
    while (query.next())
    {
        St_Points Point;
        Point.Point.setX(query.value(0).toFloat());
        Point.Point.setY(query.value(1).toFloat());
        QDataStream in(query.value(2).toByteArray());
        in>>Point.Attribute_Point;
        Point.Layer_ID=query.value(3).toInt();
        Point.PC_ID=query.value(4).toInt();
        Point.Index_Part=query.value(5).toInt();
        Container->Points_List.append(Point);
        //TODO
        QGraphicsEllipseItem *newPointCircle=
                new QGraphicsEllipseItem(Point.Point.x()-5,Point.Point.y()-5,10,10);
       // newPointCircle->setBrush(Qt::red);
        newPointCircle->setFlag(QGraphicsItem::ItemIsMovable,false);
        newPointCircle->setFlag(QGraphicsItem::ItemIsSelectable,false);
        newPointCircle->setData(0,Point.PC_ID);
        newPointCircle->setData(1,Point.Layer_ID);
        newPointCircle->setData(2,Point.Index_Part);
        Container->Items_List[Point.Layer_ID].Cur_Item.append(newPointCircle);
        area->addItem(newPointCircle);
    }
    succeed_3=query.exec(QString("select * from Line_List where ID = %1").arg(ID));
    while (query.next())
    {
        St_Lines Line;
        QDataStream in(query.value(0).toByteArray());
        in>>Line.Line_FromTo;
        QDataStream in_2(query.value(1).toByteArray());
        in_2>>Line.Attribute_Line;
        Line.Layer_ID=query.value(2).toInt();
        Line.PC_ID=query.value(3).toInt();
        Line.Index_Part=query.value(4).toInt();
        Container->Lines_List.append(Line);
        //TODO
        QPainterPath *path=new QPainterPath(Line.Line_FromTo[0]);
        for(int i=1;i<Line.Line_FromTo.count();++i)
        {
            path->lineTo(Line.Line_FromTo[i]);
        }
        QGraphicsPathItem *cur=new QGraphicsPathItem();
        cur->setPath(*path);
        //cur->setPen(*pen);
        cur->setFlag(QGraphicsItem::ItemIsMovable,false);
        cur->setFlag(QGraphicsItem::ItemIsSelectable,false);
        cur->setData(0,Line.PC_ID);
        cur->setData(1,Line.Layer_ID);
        cur->setData(2,Line.Index_Part);
        Container->Items_List[Line.Layer_ID].Cur_Item.append(cur);
        area->addItem(cur);
    }
    succeed_4=query.exec(QString("select * from Polygon_List where ID = %1").arg(ID));
    while (query.next())
    {
        St_Polygens Polygon;
        QDataStream in(query.value(1).toByteArray());
        in>>Polygon.Polygen_Round;
        QDataStream in_2(query.value(1).toByteArray());
        in_2>>Polygon.Attribute_Polygen;
        Polygon.Layer_ID=query.value(2).toInt();
        Polygon.PC_ID=query.value(3).toInt();
        Polygon.Index_Part=query.value(4).toInt();
        Container->Polygens_List.append(Polygon);
        //TODO
        QGraphicsPolygonItem *cur=new QGraphicsPolygonItem();
        QVector<QPointF>Poly_Item_Out;
        for(int i=0;i<Polygon.Polygen_Round.size();i++){
            Poly_Item_Out[i]=Polygon.Polygen_Round.at(i);
        }
        QPolygonF *curPolygon=new QPolygonF(Poly_Item_Out);
        cur->setPolygon(*curPolygon);
        cur->setFlag(QGraphicsItem::ItemIsMovable,false);
        cur->setFlag(QGraphicsItem::ItemIsSelectable,false);
    //    cur->setPen(*pen);
    //    cur->setBrush(brushColor);
        cur->setData(0,Polygon.PC_ID);
        cur->setData(1,Polygon.Layer_ID);
        cur->setData(2,Polygon.Index_Part);
        Container->Items_List[Polygon.Layer_ID].Cur_Item.append(cur);
         area->addItem(cur);
    }
    if(succeed_1||succeed_2||succeed_3||succeed_4){
        return true;
    }else
        return false;
}

bool Database::Add_Info(int ID){
    bool success_1;
    bool success_2;
    bool success_3;
    bool success_4;
    QSqlQuery query;
    //添加数据
    query.prepare("insert into Layer_List values(?,?,?,?,?,?,?,?)");
    for(int i=0;i<Container->Layers_List.size();i++){
        query.bindValue(0,Container->Layers_List.at(i).Layer_ID);
        query.bindValue(1,Container->Layers_List.at(i).Layer_Name);
        query.bindValue(2,Container->Layers_List.at(i).Ob_Type);
        query.bindValue(3,Container->Layers_List.at(i).Size);
        query.bindValue(4,Container->Layers_List.at(i).Pc_numbers);
        QByteArray Attr;
        QDataStream out(&Attr,QIODevice::WriteOnly);
        out<<Container->Layers_List.at(i).Attribute_Name;
        query.bindValue(5,Attr);
        QByteArray Every_size;
        QStringList temp_every;
        for(int j=0;j<10;j++){
            temp_every<<QString::number(Every_size[i]);
        }
        QDataStream out_2(&Every_size,QIODevice::WriteOnly);
        out_2<<temp_every;
        query.bindValue(6,Every_size);
        query.bindValue(7,ID);
        success_1=query.exec();
        if(!success_1){
            qDebug()<<"加入图层失败";
        }
    }
    query.prepare("insert into Point_List values(?,?,?,?,?,?,?)");
    for(int i=0;i<Container->Points_List.size();i++){
        query.bindValue(0,Container->Points_List.at(i).Point.x());
        query.bindValue(1,Container->Points_List.at(i).Point.y());
        QByteArray Attr;
        QDataStream out(&Attr,QIODevice::WriteOnly);
        out<<Container->Points_List.at(i).Attribute_Point;
        query.bindValue(2,Attr);
        query.bindValue(3,Container->Points_List.at(i).Layer_ID);
        query.bindValue(4,Container->Points_List.at(i).PC_ID);
        query.bindValue(5,Container->Points_List.at(i).Index_Part);

        query.bindValue(6,ID);
        success_2=query.exec();
        if(!success_2){
            qDebug()<<"加入点失败";
        }
    }
    query.prepare("insert into Line_List values(?,?,?,?,?,?)");
    for(int i=0;i<Container->Lines_List.size();i++){
        QByteArray Line;
        QDataStream out(&Line,QIODevice::WriteOnly);
        out<<Container->Lines_List.at(i).Line_FromTo;
        query.bindValue(0,Line);
        QByteArray Attr;
        QDataStream out_2(&Attr,QIODevice::WriteOnly);
        out_2<<Container->Lines_List.at(i).Attribute_Line;
        query.bindValue(1,Attr);
        query.bindValue(2,Container->Lines_List.at(i).Layer_ID);
        query.bindValue(3,Container->Lines_List.at(i).PC_ID);
        query.bindValue(4,Container->Lines_List.at(i).Index_Part);

        query.bindValue(5,ID);
        success_3=query.exec();
        if(!success_3){
            qDebug()<<"加入线失败";
        }
    }
    query.prepare("insert into Polygon_List values(?,?,?,?,?,?)");
    for(int i=0;i<Container->Polygens_List.size();i++){
        QByteArray poly;
        QDataStream out(&poly,QIODevice::WriteOnly);
        out<<Container->Polygens_List.at(i).Polygen_Round;
        query.bindValue(0,poly);
        QByteArray Attr;
        QDataStream out_2(&Attr,QIODevice::WriteOnly);
        out_2<<Container->Polygens_List.at(i).Attribute_Polygen;
        query.bindValue(1,Attr);
        query.bindValue(2,Container->Polygens_List.at(i).Layer_ID);
        query.bindValue(3,Container->Polygens_List.at(i).PC_ID);
        query.bindValue(4,Container->Polygens_List.at(i).Index_Part);

        query.bindValue(5,ID);
        success_4=query.exec();
        if(!success_4){
            qDebug()<<"加入面失败";
        }
    }
    if(success_1||success_2||success_3||success_4){
        return true;
    }else
        return false;
}

void Database::Delete_Info(int ID){
    QSqlQuery query;
    query.prepare("delete * TABLE from Layer_List where ID=?");
    query.bindValue(0,ID);
    query.exec("delete *  TABLE from Point_List where ID=?");
    query.bindValue(0,ID);
    query.exec("delete *  TABLE from Line_List where ID=?");
    query.bindValue(0,ID);
    query.exec("delete *  TABLE from Polygon_List where ID=?");
    query.bindValue(0,ID);
}
