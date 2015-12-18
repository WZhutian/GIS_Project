#ifndef STRUCT_LIST
#define STRUCT_LIST
#include <QList>
#include <QString>
#include <QPointF>
#include <QImage>
//其中，删除以后，Index_Part值不变，每次添加都按照layers的Size+Delete_Times
struct St_Points{//点
    QPointF Point;//存储点坐标
    QList<QString> Attribute_Point;//存储属性
    int Layer_ID;//图层标记
    int PC_ID;//客户端标记,当前PC不属于该ID的时候，禁止编辑！！
    int Index_Part;//在当前PC号对应部分下的索引号
};
struct St_Lines{//线与折线
    QList<QPointF> Line_FromTo;//按顺序存储折线段的上点
    QList<QString> Attribute_Line;
    int Layer_ID;//图层标记
    int PC_ID;//客户端标记，当前PC不属于该ID的时候，禁止编辑！！
    int Index_Part;//在当前PC号对应部分下的索引号

};
struct St_Polygens{//多边形
    QList<QPointF> Polygen_Round;//顺时针存储多边形上的点
    QList<QString> Attribute_Polygen;
    int Layer_ID;//图层标记
    int PC_ID;//客户端标记，当前PC不属于该ID的时候，禁止编辑！！
    int Index_Part;//在当前PC号对应部分下的索引号

};
struct St_Raster_images{//栅格图片
    QList<QPointF> Image_Point;//图片四个角的位置
    QImage *Image;
    int Layer_ID;//图层标记
};
/**
 * @brief The St_Change struct
 * 每次增删改都要修改相应图层下
 * 负责人：W
 */
struct St_Change{//保存TCP传输过程中需要重发的图元信息
    int PC_ID;//修改地方是被哪一个PC所修改的
    int Index_Part;//修改的索引号
    QList<int> Sended_PC_ID;//服务端一直保存的，发送给哪个PC就添加对应PC_ID号，下次就不发送了
    int Change_Way;//0为删除，1为修改，2为添加
};
/**
 * @brief The St_Layers struct
 * 用于图层显示以及tcp过程中的首要判断要素
 * 删除要素时候需要添加删除次数
 * 负责人：W
 */
struct St_Layers{//图层
    int Layer_ID;//（未必需要）与list中的索引号冲突
    QString Layer_Name;//图层名字
    int Pc_numbers=0;//保存当前工作集群中的PC数量
    int Every_size[10] ={0};//按顺序保存各个PC所对应的图元数量，假设不超过10个PC
    int Size=0;//保存图层中图元的数量
    int Ob_Type;//图元类型，0为点，1为线，2为面
    bool Visiable=false;//是否显示
    bool isEditing=false;//是否为当前编辑
    int Delete_Times=0;//删除次数
    //TCP通信判断部分：
    QList<St_Change> Change_List;
    //被修改/删除的图元在第三层PC_ID对应部分中的索引号，用于tcp中重发机制，
    //客户机每次发送都清空一次，告诉服务端它此次上传修改了那些地方，服务端永久保存
    //只用作从服务端获取更新
};


#endif // STRUCT_LIST

