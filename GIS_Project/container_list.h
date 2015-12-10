#ifndef CONTAINER_LIST_H
#define CONTAINER_LIST_H
#include <struct_list.h>
#include <QList>

class Container_List
{
public:
    Container_List();
    QList<Points> Points_List;
    QList<Lines> Lines_List;
    QList<Polygens> Polygens_List;
    int Layer_numbers;//当前图层数量
};

#endif // CONTAINER_LIST_H
