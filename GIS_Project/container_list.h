#ifndef CONTAINER_LIST_H
#define CONTAINER_LIST_H
#include <struct_list.h>
#include <QList>
#include <QPointF>
class Container_List
{
public:
    Container_List();
    int PC_ID;
    int Layer_ID;//当前编辑的图层号

    QList<St_Points> Points_List;
    QList<St_Lines> Lines_List;
    QList<St_Polygens> Polygens_List;
    QList<St_Raster_images> Images_List;
    QList<St_Layers> Layers_List;//图层列表，用以映射以上list
    ////////////////////////增删改//////////////////////////////////
    void Add_Point(QPointF Point_Out);
    void Modify_Point(int Index_Part_Out,QPointF Point_Out_New);
    void Delete_Point(int Index_Part_Out);
    void Add_Line(QList<QPointF> Line_Out);
    void Modify_Line(int Index_Part_Out,int Index_Line,QPointF Line_Out_New);
    void Delete_Line(int Index_Part_Out);
    void Add_Polygen(QList<QPointF> Polygen_Out);
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
};

#endif // CONTAINER_LIST_H
