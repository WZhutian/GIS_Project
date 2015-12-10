#ifndef STRUCT_LIST
#define STRUCT_LIST
#include <QList>
#include <QString>
#include <QPointF>
struct St_Points{//点
    QPointF Point;//存储点坐标
    QList<QString> Attribute_Point;//存储属性
    int Layer_ID;

};
struct St_Lines{//线与折线
    QList<QPointF> Line_FromTo;//按顺序存储折线段的上点
    QList<QString> Attribute_Line;
    int Layer_ID;

};
struct St_Polygens{//多边形
    QList<QPointF> Polygen_Round;//顺时针存储多边形上的点
    QList<QString> Attribute_Polygen;
    int Layer_ID;

};
struct St_Raster_images{//栅格图片
    QList<QString> Attribute_Point;
    int Layer_ID;

};
struct St_Layers{//图层
    int Layer_ID;
    QString Layer_Name;
    int Index_From;//保存该图层在QList中的索引号范围，从0开始计数
    int Index_To;
    //TODO 图层类型
};

#endif // STRUCT_LIST

