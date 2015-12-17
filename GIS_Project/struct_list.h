#ifndef STRUCT_LIST
#define STRUCT_LIST
#include <QList>
#include <QString>
#include <QPointF>
struct St_Points{//点
    QPointF Point;//存储点坐标
    QList<QString> Attribute_Point;//存储属性
    int Layer_ID;//图层标记
    int PC_ID;//客户端标记
    int Index_Part;//在当前PC号对应部分下的索引号
};
struct St_Lines{//线与折线
    QList<QPointF> Line_FromTo;//按顺序存储折线段的上点
    QList<QString> Attribute_Line;
    int Layer_ID;//图层标记
    int PC_ID;//客户端标记
    int Index_Part;//在当前PC号对应部分下的索引号

};
struct St_Polygens{//多边形
    QList<QPointF> Polygen_Round;//顺时针存储多边形上的点
    QList<QString> Attribute_Polygen;
    int Layer_ID;//图层标记
    int PC_ID;//客户端标记
    int Index_Part;//在当前PC号对应部分下的索引号

};
struct St_Raster_images{//栅格图片
    QList<QPointF> Image_Point;//图片四个角的位置
    //TODO 存储图片
    int Layer_ID;//图层标记
    int PC_ID;//客户端标记
    int Index_Part;//在当前PC号对应部分下的索引号

};
struct St_Layers{//图层
    int Layer_ID;//（未必需要）
    QString Layer_Name;//图层名字
    int Pc_numbers;//保存当前工作集群中的PC数量
    QList<int> Every_size;//按顺序保存各个PC所对应的图元数量
    int Index_From;//保存该图层在QList中的索引号范围，从0开始计数
    int Index_To;
    int Ob_Type;//图元类型，0为点，1为线，2为面
    bool Visiable;//是否显示
    bool isEditing;//是否为当前编辑
};

#endif // STRUCT_LIST

