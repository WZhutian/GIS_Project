#ifndef STRUCT_LIST
#define STRUCT_LIST
#include <QList>
#include <QString>
#include <QPointF>
struct St_Points{//点
    QPointF Point;//存储点坐标
    QList<QString> Attribute_Point;//存储属性
    int PC_ID;//客户端标记

};
struct St_Lines{//线与折线
    QList<QPointF> Line_FromTo;//按顺序存储折线段的上点
    QList<QString> Attribute_Line;
    int PC_ID;//客户端标记

};
struct St_Polygens{//多边形
    QList<QPointF> Polygen_Round;//顺时针存储多边形上的点
    QList<QString> Attribute_Polygen;
    int PC_ID;//客户端标记

};
struct St_Raster_images{//栅格图片
    QList<QString> Attribute_Point;
    int PC_ID;//客户端标记

};
struct St_Layers{//图层
    int Layer_ID;
    QString Layer_Name;
    int Index_From;//保存该图层在QList中的索引号范围，从0开始计数
    int Index_To;
    int Ob_Type;//图元类型，0为点，1为线，2为面
    bool visiable;//是否显示
};
struct St_Operates{//操作记录
    int Ob_Type;//图元类型，0为点，1为线，2为面
    int Op_Type;//操作类型,1为增，1为删，2为改

};

#endif // STRUCT_LIST

