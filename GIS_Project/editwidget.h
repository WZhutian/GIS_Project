#ifndef EDITWIDGET_H
#define EDITWIDGET_H
#include<gdal_readfile.h>
#include <QGraphicsScene>
#include <QLabel>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include<QPointF>
#include<container_list.h>
#include<QGraphicsSceneWheelEvent>
#include<QWheelEvent>
class Container_List;
class EditWidget: public QGraphicsScene
{
public:
    EditWidget(QWidget *parent=0);
    Container_List *Container;
    bool isDrawing;      //false表示结束画图，true表示正在画图
    bool isEditing;
    int editPointIndex;
    QPointF movePoint;//移动对象时移动的点
    QPointF origPoint;//移动对象时的原始点
    QPointF origPoint1;//移动场景的原始点
    QPointF movePoint1;//移动场景的移动点
    QPen *pen;

    QGraphicsItem* curEditItem;
    QGraphicsItem* curdrawitem;
    void Get_Container(Container_List &Container_Out);
    void Get_Graphicview(QGraphicsView &Graphic_View_Out);
    QGraphicsView *view;
    enum ShapeType   //枚举变量,要绘制的图形的类型
    {
        NoneType,
        PointType,
        LineType,
        RectangleType,
        EllispeType,
        PolylineType,
        PolygonType
    };

    enum viewState
    {
        NoneState,
        DrawType,
       // MoveType,
        EditType,
        MoveSceneType,
        RotateType,
        clearType
    };

    void setPenStyle(Qt::PenStyle style); //设置画笔风格
    void setPenWidth(int width);   //设置画笔宽度
    void setPenColor(QColor color);   //设置画笔颜色
    void setBrushColor(QColor color);   //设置填充颜色
    void setShape(ShapeType shape);  //设置要绘制的图形
    void setState(viewState state);
    void showShape(QList<QGraphicsItem*> &shapes,int all=0);
    void setPen();
    QVector<QPointF> points;    //绘制的图形对应的坐标
    QList<QGraphicsItem*> shapes;//交互操作图形容器

    QVector<QPointF> points2; //存从数据库读出来的点
    QVector<QPointF> points3;
    void showpoints();
    void showpicture();
    void showNet();
    void showlines(QList<QPointF>linespoints);
    void showpologon(QVector<QPoint>pologonpoints);

    ~EditWidget();

protected:
//    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *);   //鼠标移动事件
    void mousePressEvent(QGraphicsSceneMouseEvent *);   //鼠标按下事件
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);   //鼠标释放事件
private:
    QColor penColor;    //画笔颜色
    QColor brushColor;   //填充颜色
    int penWidth;     //画笔宽度
//    int itemsize;    //item数量
    Qt::PenStyle penStyle;    //画笔风格

    ShapeType curShape;
    viewState curState;


    void mousePressClear(QGraphicsSceneMouseEvent *);
    void mousePressDraw(QGraphicsSceneMouseEvent *);
    //void mousePressMove(QGraphicsSceneMouseEvent *);
    void mousePressEdit(QGraphicsSceneMouseEvent *);
    void mousePressMoveScene(QGraphicsSceneMouseEvent *);

    void mouseMoveDraw(QGraphicsSceneMouseEvent *);
   // void mouseMoveMove(QGraphicsSceneMouseEvent *);
    void mouseMoveEdit(QGraphicsSceneMouseEvent *);
    void mouseMoveClear(QGraphicsSceneMouseEvent *);
    void mouseMoveMoveScene(QGraphicsSceneMouseEvent *);

    void mouseReleaseDraw(QGraphicsSceneMouseEvent *);
    //void mouseReleaseMove(QGraphicsSceneMouseEvent *);
    void mouseReleaseEdit(QGraphicsSceneMouseEvent *);
    void mouseReleaseClear(QGraphicsSceneMouseEvent *);
    void mouseReleaseMoveScene(QGraphicsSceneMouseEvent *);

    void mouseDoubleClickEdit(QGraphicsSceneMouseEvent *);
};

#endif // EDITWIDGET_H
