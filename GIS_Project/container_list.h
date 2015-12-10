#ifndef CONTAINER_LIST_H
#define CONTAINER_LIST_H
#include <struct_list.h>
#include <QList>

class Container_List
{
public:
    Container_List();
    QList<St_Points> Points_List;
    QList<St_Lines> Lines_List;
    QList<St_Polygens> Polygens_List;
    QList<St_Layers> Layers_List;//图层列表，用以映射以上list
};

#endif // CONTAINER_LIST_H
