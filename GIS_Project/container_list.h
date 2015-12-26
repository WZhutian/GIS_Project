#ifndef CONTAINER_LIST_H
#define CONTAINER_LIST_H
#include <struct_list.h>
#include <QList>
#include <QPointF>
#include<QVector>
#include<QGraphicsItem>
#include "editwidget.h"
class EditWidget;
class Container_List
{
public:
    Container_List();
    EditWidget *area;
    void Get_Area(EditWidget& Area_out){
        area=&Area_out;
    }

    ////
    int PC_ID;
    int Layer_ID;//当前编辑的图层号


    ///
    QList<St_Items> Items_List;
    QList<St_Points> Points_List;
    QList<St_Lines> Lines_List;
    QList<St_Polygens> Polygens_List;
    QList<St_Raster_images> Images_List;
    QList<St_Layers> Layers_List;//图层列表，用以映射以上list
    ////////////////////////增删改//////////////////////////////////
    void Add_Point(QPointF Point_Out);
    void Modify_Point(int Index_Part_Out,QPointF Point_Out_New);
    void Delete_Point(int Index_Part_Out);
    void Add_Line(QVector<QPointF> Line_Out);
    void Modify_Line(int Index_Part_Out,int Index_Line,QPointF Line_Out_New);
    void Delete_Line(int Index_Part_Out);
    void Add_Polygen(QVector<QPointF> Polygen_Out);
    void Modify_Polygen(int Index_Part_Out,int Index_Polygen,QPointF Polygen_Out_New);
    void Delete_Polygen(int Index_Part_Out);
    ///////////////////////添加图层
    void Add_Layer(QString Layer_Name,int Ob_Type);
    void Add_Layer_Attr(int Layer_ID_Out,QString Attribute_Name);
    ////////////////////////查找/////////////////////////////
    int Current_search(int Layer_ID,int PC_ID,int Index_Part,int Type);
    int Add_search(int Layer_ID,int PC_ID,int Type);
    int Search_Layer_Index(int Layer_ID_Out);
    int Current_insert(int Layer_ID,int PC_ID,int Index_Part,int Type);
    ////////////////////////添加Items//////////////////////////////////
    void Add_Point_Item(QPointF Point_Item_Out,int index);
    void Add_Line_Item(QVector<QPointF> Line_Item_Out,int index);
    void Add_Polygen_Item(QVector<QPointF> Poly_Item_Out,int index);
    //////////////////////修改Items////////////////////////////////////
    void Modify_Point_Item(QPointF Point_Item_Out_New,int index);
    void Modify_Line_Item(QPointF Line_Item_Out_new,int index,int index_Line);
    void Modify_Polygen_Item(QPointF Polygens_Item_Out_new,int index,int Index_Polygen);
    //////////////////////删除Items/////////////////////////////////////
    void Delete_Point_Item(int Index);
    void Delete_Line_Item(int Index);
    void Delete_Polygen_Item(int Index);

};

#endif // CONTAINER_LIST_H
