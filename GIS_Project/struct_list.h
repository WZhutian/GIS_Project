#ifndef STRUCT_LIST
#define STRUCT_LIST
#include <QList>
#include <QString>
#include <QPointF>
struct Points{//点
    QPointF Point;//存储点坐标
    QList<QString> Attribute_Point;//存储属性
    int Layer_ID;

};
struct Lines{//线与折线
    QList<QPointF> Line_FromTo;//按顺序存储折线段的上点
    QList<QString> Attribute_Line;
    int Layer_ID;

};
struct Polygens{//多边形
    QList<QPointF> Polygen_Round;//顺时针存储多边形上的点
    QList<QString> Attribute_Polygen;
    int Layer_ID;

};
struct Raster_images{//栅格图片
    QList<QString> Attribute_Point;
    int Layer_ID;

};

#endif // STRUCT_LIST

