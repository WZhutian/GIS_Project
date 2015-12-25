#include "editwidget.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>
#include <QPainterPath>
#include <QMessageBox>
#include <math.h>
#include <QDebug>
#include<QImage>
#include<QScrollBar>
EditWidget::EditWidget(QWidget *parent):QGraphicsScene(parent)
{
    curShape = this->NoneType;
    curState=this->MoveSceneType;
    isDrawing=false;
    isEditing=false;

    penColor = Qt::black;
    brushColor = Qt::black;
    penWidth = 1;
    penStyle = Qt::SolidLine;
    pen=new QPen();
    editPointIndex=-1;

}
void EditWidget::Get_Container(Container_List &Container_Out){
    Container=&Container_Out;
}
void EditWidget::Get_Graphicview(QGraphicsView &Graphic_View_Out){
    view=&Graphic_View_Out;
}

void EditWidget::mousePressDraw(QGraphicsSceneMouseEvent *event)
{
    setPen();
    if(curShape==this->PointType)
    {
                    if(event->button()==Qt::LeftButton && this->isDrawing)
                    {
//                         for(int i=0;i<points.count();i++)
//                         {
//                             this->removeItem(this->items()[0]);
//                         }
//                        points.append(event->scenePos());
//                        for(int i=0;i<points.count();i++)
//                        {
//                            QGraphicsEllipseItem *newPointCircle=
//                                    new QGraphicsEllipseItem(points[i].x()-5,points[i].y()-5,10,10);
//                            newPointCircle->setBrush(Qt::red);
//                            newPointCircle->setFlag(QGraphicsItem::ItemIsMovable,true);
//                            newPointCircle->setFlag(QGraphicsItem::ItemIsSelectable,true);
//                            Container->Items_List[Container->Layer_ID].Cur_Item.append(newPointCircle);
//                            editPointIndex=i;
//                        }
                        Container->Add_Point(event->scenePos());
                          showShape(Container->Items_List[Container->Layer_ID].Cur_Item);
                         // shapes.clear();
                          isDrawing=false;
//                        for(int i=0;i<points.count();i++)
//                        {
//                            Container->Add_Point(points[i]);
//                        }

                         // qDebug()<<this->items().size();
                    }
                    else if(event->button()==Qt::LeftButton && !this->isDrawing)     //如果之前没有开始画图，则单击开始画图
                    {
                        //points.clear();
                       // points.append(event->scenePos());

//                        QGraphicsEllipseItem *newPointCircle=
//                                new QGraphicsEllipseItem(points[0].x()-5,points[0].y()-5,10,10);
//                        newPointCircle->setBrush(Qt::red);
//                        newPointCircle->setFlag(QGraphicsItem::ItemIsMovable,true);
//                        newPointCircle->setFlag(QGraphicsItem::ItemIsSelectable,true);
//                          Container->Items_List[Container->Layer_ID].Cur_Item.append(newPointCircle);
                        Container->Add_Point(event->scenePos());
                         showShape( Container->Items_List[Container->Layer_ID].Cur_Item);
                         //shapes.clear();
                        isDrawing=true;
                    }
    }
    else if(curShape==this->PolylineType)
    {
        if(event->button()==Qt::LeftButton && this->isDrawing)
        {


            points.append(event->scenePos());
            QPainterPath *path=new QPainterPath(points[0]);
            for(int i=1;i<points.count();++i)
            {
                path->lineTo(points[i]);
            }
            QGraphicsPathItem *cur=new QGraphicsPathItem();
            cur->setPath(*path);
            //                QGraphicsPolylineItem *cur=new QGraphicsPolylineItem(points);
            cur->setPen(*pen);
            cur->setFlag(QGraphicsItem::ItemIsMovable,true);
            cur->setFlag(QGraphicsItem::ItemIsSelectable,true);
            shapes.append(cur);
            showShape(shapes);
            shapes.clear();
            this->removeItem(this->items()[1]);
           // qDebug()<<this->items().size();
        }
        else if(event->button()==Qt::LeftButton && !this->isDrawing)     //如果之前没有开始画图，则单击开始画图
        {
            points.clear();
            points.append(event->scenePos());
            QGraphicsPathItem *cur=new QGraphicsPathItem();
             QPainterPath *path=new QPainterPath(points[0]);
             cur->setPath(*path);
             cur->setPen(*pen);
             shapes.append(cur);
             showShape(shapes);
             shapes.clear();
            isDrawing=true;
        }
        else if(event->button()==Qt::RightButton && this->isDrawing) //如果正在画图，右键单击结束画图
        {
//            QPainterPath *path=new QPainterPath(points[0]);
//            for(int i=1;i<points.count();++i)
//            {
//                path->lineTo(points[i]);
//            }
//            QGraphicsPathItem *cur=new QGraphicsPathItem();
//            cur->setPath(*path);
//            //                QGraphicsPolylineItem *cur=new QGraphicsPolylineItem(points);
//            cur->setFlag(QGraphicsItem::ItemIsMovable,true);
//            cur->setFlag(QGraphicsItem::ItemIsSelectable,true);
//            cur->setPen(*pen);
//            shapes.append(cur);
//            showShape(shapes);
//            shapes.clear();

            Container->Add_Line(points);
            showShape(Container->Items_List[Container->Layer_ID].Cur_Item);

            this->removeItem(this->items()[1]);
            isDrawing=false;

            qDebug()<<Container->Items_List[Container->Layer_ID].Cur_Item.size();
        }
    }
    else if(curShape==this->PolygonType)
    {

        if(event->button()==Qt::LeftButton && this->isDrawing)
        {
            points.append(event->scenePos());
            QGraphicsPolygonItem *cur=new QGraphicsPolygonItem();
            QPolygonF *curPolygon=new QPolygonF(points);
            cur->setPolygon(*curPolygon);
            cur->setPen(*pen);
            cur->setBrush(brushColor);

            shapes.append(cur);
            showShape(shapes);
            this->removeItem(this->items()[1]);
            shapes.clear();
        }
        else if(event->button()==Qt::LeftButton && !this->isDrawing)     //如果之前没有开始画图，则单击开始画图
        {

            points.clear();
            points.append(event->scenePos());
            QGraphicsPathItem *cur=new QGraphicsPathItem();
             QPainterPath *path=new QPainterPath(points[0]);
             cur->setPath(*path);
             cur->setPen(*pen);
             shapes.append(cur);
             showShape(shapes);
             shapes.clear();
            isDrawing=true;
        }
        else if(event->button()==Qt::RightButton && this->isDrawing)
        {
//            QGraphicsPolygonItem *cur=new QGraphicsPolygonItem();
//            QPolygonF *curPolygon=new QPolygonF(points);
//            cur->setPolygon(*curPolygon);
//            cur->setFlag(QGraphicsItem::ItemIsMovable,true);
//            cur->setFlag(QGraphicsItem::ItemIsSelectable,true);
//            cur->setPen(*pen);
//            cur->setBrush(brushColor);
//            shapes.append(cur);
//            showShape(shapes);
//            shapes.clear();

             Container->Add_Polygen(points);
            this->removeItem(this->items()[1]);
            isDrawing=false;

        }
    }
}

void EditWidget::mousePressMove(QGraphicsSceneMouseEvent *event)
{
    this->views()[0]->setCursor(Qt::ClosedHandCursor);
    if(this->items().count()>0)
    {

        QGraphicsItem* curSelectItem = this->itemAt(event->scenePos(), this->items()[0]->transform());
        if(curSelectItem != NULL)
        {
            this->clearSelection();
            origPoint=event->scenePos();
            curSelectItem->setSelected(true);


        }
    }
}

void EditWidget::mousePressEdit(QGraphicsSceneMouseEvent *event)
{

    if(event->button()==Qt::LeftButton && this->isEditing)
    {
        this->views()[0]->setCursor(Qt::SizeAllCursor);
        QPointF pressPoint=event->scenePos();
        if(points.count()>1)
        {
            for(int i=0;i<points.count();++i)
            {
                if(abs(pressPoint.x()-points[i].x())<5 && abs(pressPoint.y()-points[i].y())<5)
                {
                    QGraphicsEllipseItem *newPointCircle=
                            new QGraphicsEllipseItem(points[i].x()-5,points[i].y()-5,10,10);
                    newPointCircle->setBrush(Qt::red);
                    QGraphicsItem *origPointCricle=
                            this->itemAt(points[i],this->items()[0]->transform());
                    // shapes.removeOne(origPointCricle);
                    this->removeItem(origPointCricle);
                    shapes.append(newPointCircle);
                    showShape(shapes);
                    shapes.clear();
                    editPointIndex=i;
                }
            }
        }
      }

    if(event->button()==Qt::LeftButton && editPointIndex==-1 && this->items().count()>0&&isEditing==false)     //如果之前没有开始编辑，则单击开始编辑
    {
        this->views()[0]->setCursor(Qt::PointingHandCursor);
        points.clear();
        curEditItem = this->itemAt(event->scenePos(), this->items()[0]->transform());
        //shapes=shapes;
        QGraphicsEllipseItem *curEditPointCircleItem=dynamic_cast<QGraphicsEllipseItem*>(curEditItem);
        if(curEditPointCircleItem!=NULL)
        {     //qDebug()<<this->items().count();
            QPointF curPoint1(curEditPointCircleItem->rect().topLeft().x()+5,
                             curEditPointCircleItem->rect().topLeft().y()+5);
            points.append(curPoint1);

            this->clearSelection();
            origPoint=event->scenePos();
            curEditItem->setSelected(true);
             editPointIndex=0;
        }
        QGraphicsPathItem*curEditPathItem=dynamic_cast<QGraphicsPathItem*>(curEditItem);
        if(curEditPathItem!=NULL&&curEditPathItem->path().elementCount()>1)
        {
            for(int i=0;i<curEditPathItem->path().elementCount();++i)
            {
                QPointF curPoint(curEditPathItem->path().elementAt(i).x,
                                 curEditPathItem->path().elementAt(i).y);
                points.append(curPoint);
                QGraphicsEllipseItem *pointCircle=new
                        QGraphicsEllipseItem(curPoint.x()-3,curPoint.y()-3,6,6);
                pointCircle->setBrush(Qt::red);
                shapes.append(pointCircle);
                showShape(shapes);
                shapes.clear();
            }
        }
        QGraphicsPolygonItem*curEditPolygonItem=dynamic_cast<QGraphicsPolygonItem*>(curEditItem);
        if(curEditPolygonItem!=NULL)
        {
            for(int i=0;i< curEditPolygonItem->polygon().count();++i)
            {
                points.append(curEditPolygonItem->polygon().data()[i]);
                QGraphicsEllipseItem *pointCircle=new
                        QGraphicsEllipseItem(points[i].x()-3,points[i].y()-3,6,6);
                pointCircle->setBrush(Qt::red);
                shapes.append(pointCircle);
                showShape(shapes);
                shapes.clear();
            }
        }
        isEditing=true;

    }
    else if(event->button()==Qt::RightButton && this->isEditing)
    {
        this->views()[0]->setCursor(Qt::PointingHandCursor);
//        showShape(shapes);
        for(int i=0;i<points.count();i++)
        {
            this->removeItem(this->items()[0]);
        }
        isEditing=false;
        points.clear();
        shapes.clear();
        curEditItem=NULL;
        editPointIndex=-1;
    }

}
void EditWidget::mousePressMoveScene(QGraphicsSceneMouseEvent *event)
{
    this->views()[0]->setCursor(Qt::ClosedHandCursor);
     //origPoint1=event->scenePos();
    if(( event->buttons()&Qt::LeftButton)&& isDrawing==false )
    {
        this->views()[0]->setDragMode(QGraphicsView::ScrollHandDrag);
    }
}


void EditWidget::mouseMoveDraw(QGraphicsSceneMouseEvent *event)
{
    this->views()[0]->setCursor(Qt::CrossCursor);
    if(isDrawing)
    {
        setPen();
        if(curShape==this->PointType)
        {
            isDrawing=false;
        }
        else if(curShape==this->PolylineType)
        {
            if(this->items().size()!=0)
            {
                //curdrawitem=this->itemAt(lastPoint,this->items()[0]->transform());
                curdrawitem=this->items()[0];

                if(curdrawitem!=NULL)
                {

                    this->removeItem(curdrawitem);
                }
            }
            QVector<QPointF> tempPoints=points;
            tempPoints.append(event->scenePos());
            QPainterPath *path=new QPainterPath(tempPoints[0]);
            for(int i=1;i<tempPoints.count();++i)
            {
                path->lineTo(tempPoints[i]);
            }
            QGraphicsPathItem *cur=new QGraphicsPathItem();
            cur->setPath(*path);
            cur->setPen(*pen);
            cur->setFlag(QGraphicsItem::ItemIsMovable,true);
            cur->setFlag(QGraphicsItem::ItemIsSelectable,true);
            shapes.append(cur);
            showShape(shapes);
            shapes.clear();

        }
        else if(curShape==this->PolygonType)
        {
            if(this->items().size()!=0)
            {
                //curdrawitem=this->itemAt(lastPoint,this->items()[0]->transform());
                curdrawitem=this->items()[0];

                if(curdrawitem!=NULL)
                {

                    this->removeItem(curdrawitem);
                }
            }
            QVector<QPointF> tempPoints=points;
            tempPoints.append(event->scenePos());
            QGraphicsPolygonItem *cur=new QGraphicsPolygonItem();
            QPolygonF *curPolygon=new QPolygonF(tempPoints);
            cur->setPolygon(*curPolygon);
            cur->setPen(*pen);
            cur->setBrush(brushColor);
         //   shapes=shapes;
            shapes.append(cur);
            showShape(shapes);
            shapes.clear();
        }
    }
}

void EditWidget::mouseMoveMove(QGraphicsSceneMouseEvent *event)
{
    if(( event->buttons()&Qt::LeftButton)&& isDrawing==false && this->selectedItems().count()>0)
    {
        //  qDebug()<<this->selectedItems().count();
        movePoint=event->scenePos();
        qreal dx=movePoint.x()-origPoint.x();
        qreal dy=movePoint.y()-origPoint.y();
        for(int i=0;i<this->selectedItems().count();++i)
        {
            this->selectedItems()[i]->moveBy(dx,dy);
        }
        origPoint=movePoint;
    }

}
void EditWidget::mouseMoveMoveScene(QGraphicsSceneMouseEvent *event)
{
   // if(( event->buttons()&Qt::LeftButton)&& isDrawing==false )
  // {

//        movePoint1=event->scenePos();
//        qreal dx=movePoint1.x()-origPoint1.x();
//        qreal dy=movePoint1.y()-origPoint1.y();



//        qreal x=view->contentsRect().x()-dx;
//        qreal y=view->contentsRect().y()-dy;
//        QScrollBar *pVScrollBar=new QScrollBar();
//        pVScrollBar=view->verticalScrollBar();
//        if(pVScrollBar!=NULL)//竖直方向调整
//        {
//            qreal nYValue =pVScrollBar->value();
//            pVScrollBar->setValue(nYValue+y);

//        }
//        QScrollBar *pHScrollBar =view->horizontalScrollBar();
//        if(pHScrollBar!=NULL)
//        {
//            qreal nXValue=pHScrollBar->value();
//            pHScrollBar->setValue(nXValue+x);
//        }
//        view->show();
        //origPoint1=movePoint1;
 //  }
}

void EditWidget::mouseMoveEdit(QGraphicsSceneMouseEvent *event)
{
    if((event->buttons()&Qt::LeftButton)&& editPointIndex!=-1)
    {
        movePoint=event->scenePos();
        if(points.count()<2&&this->selectedItems().count()>0)
        {
            qDebug()<<this->selectedItems().count();
            qreal dx=movePoint.x()-origPoint.x();
            qreal dy=movePoint.y()-origPoint.y();
            for(int i=0;i<this->selectedItems().count();++i)
            {
                this->selectedItems()[i]->moveBy(dx,dy);
            }
            origPoint=movePoint;
            isEditing=false;
        }
        if(points.count()>1)
        {

        points[editPointIndex]=event->scenePos();

       QGraphicsPathItem*curEditPathItem=dynamic_cast<QGraphicsPathItem*>(curEditItem);
       if(curEditPathItem!=NULL&&curEditPathItem->path().elementCount()>1)
       {
//            QPainterPath *path=new QPainterPath(points[0]);
//            for(int i=1;i<points.count();++i)
//            {
//                path->lineTo(points[i]);
//            }
//            curEditPathItem->setPath(*path);
        Container->Modify_Line(curEditItem->data(2).toInt(),editPointIndex,event->scenePos());

        }
        QGraphicsPolygonItem*curEditPolygonItem=dynamic_cast<QGraphicsPolygonItem*>(curEditItem);
        if(curEditPolygonItem!=NULL)
        {
//            QPolygonF *curPolygon=new QPolygonF(points);
//            curEditPolygonItem->setPolygon(*curPolygon);
            Container->Modify_Polygen(curEditItem->data(2).toInt(),editPointIndex,event->scenePos());
        }
        //shapes=shapes;
        for(int i=0;i<points.count();++i)
        {

            if(i==editPointIndex)
            {

                    QGraphicsEllipseItem *pointCircle=new
                            QGraphicsEllipseItem(points[i].x()-5,points[i].y()-5,10,10);
                    pointCircle->setBrush(Qt::red);
                    shapes.append(pointCircle);
                    showShape(shapes);
                    shapes.clear();
            }
            else
            {
                QGraphicsEllipseItem *pointCircle=new
                        QGraphicsEllipseItem(points[i].x()-3,points[i].y()-3,6,6);
                pointCircle->setBrush(Qt::red);
                shapes.append(pointCircle);
                showShape(shapes);
                shapes.clear();
            }
       }
        for(int i=0;i<points.count();i++)
        {
            this->removeItem(this->items()[0]);
        }

    }
    }
}

void EditWidget::mouseReleaseDraw(QGraphicsSceneMouseEvent *)
{

}

void EditWidget::mouseReleaseMove(QGraphicsSceneMouseEvent *)
{
    this->views()[0]->setCursor(Qt::OpenHandCursor);
    this->clearSelection();

}
void EditWidget::mouseReleaseMoveScene(QGraphicsSceneMouseEvent *event)
{
    this->views()[0]->setCursor(Qt::OpenHandCursor);
    if(( event->buttons()&Qt::LeftButton)&& isDrawing==false )
    {
        this->views()[0]->setDragMode(QGraphicsView::NoDrag);
    }

}

void EditWidget::mouseReleaseEdit(QGraphicsSceneMouseEvent *event)
{
    if(points.count()>1)
    {
    this->views()[0]->setCursor(Qt::PointingHandCursor);
//    shapes=shapes;
    for(int i=0;i<points.count();++i)
    {
        QGraphicsEllipseItem *pointCircle=new
                QGraphicsEllipseItem(points[i].x()-5,points[i].y()-5,10,10);
        pointCircle->setBrush(Qt::red);
        shapes.append(pointCircle);
    }
    editPointIndex=-1;
    for(int i=0;i<points.count();i++)
    {
        this->removeItem(this->items()[0]);
    }
    showShape(shapes);
  shapes.clear();
    }
    if(points.count()<2)
    {
        this->views()[0]->setCursor(Qt::PointingHandCursor);
        this->clearSelection();
        isEditing=false;
        editPointIndex=-1;
        QPointF ChangePoint =event->scenePos();
        Container->Modify_Point(curEditItem->data(2).toInt(),ChangePoint);
    }
}

void EditWidget::mouseDoubleClickEdit(QGraphicsSceneMouseEvent *)
{

}
void EditWidget::mouseMoveClear(QGraphicsSceneMouseEvent *)
{

}

void EditWidget::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    if(curState==DrawType)
    {
        mousePressDraw(event);
    }
    else if(curState==MoveType)
    {
        mousePressMove(event);
    }
    else if(curState==EditType)
    {
        mousePressEdit(event);
    }
    else if(curState==clearType)
    {
        mousePressClear(event);
    }
    else if(curState==MoveSceneType)
    {
        mousePressMoveScene(event);
    }
}


void EditWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

    if(curState==DrawType)
    {
        mouseMoveDraw(event);
    }
    else if(curState==MoveType)
    {
        mouseMoveMove(event);
    }
    else if(curState==EditType)
    {
        mouseMoveEdit(event);
    }
    else if(curState==clearType)
    {
        mouseMoveClear(event);
    }
    else if(curState==MoveSceneType)
    {
        mouseMoveMoveScene(event);
    }
}


void EditWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    if(curState==DrawType)
    {
        mouseReleaseDraw(event);
    }
    else if(curState==MoveType)
    {
        mouseReleaseMove(event);
    }
    else if(curState==EditType)
    {
        mouseReleaseEdit(event);
    }
    else if(curState==MoveSceneType)
    {
        mouseReleaseMoveScene(event);
    }
}

void EditWidget::showShape(QList<QGraphicsItem *>&shapes)
{



        this->addItem(shapes[0]);

}


void EditWidget::setPen()
{
    pen->setStyle(penStyle);
    pen->setWidth(penWidth);
    pen->setColor(penColor);
}

void EditWidget::setPenStyle(Qt::PenStyle style)
{
    penStyle=style;
}

void EditWidget::setPenColor(QColor color)
{
    penColor=color;
}

void EditWidget::setPenWidth(int width)
{
    penWidth=width;
}

void EditWidget::setBrushColor(QColor color)
{
    brushColor=color;
}
void EditWidget::setShape(ShapeType shape)
{
    curShape=shape;
}
void EditWidget::setState(viewState state)
{
    curState=state;
}

void EditWidget::showpoints()
{

    for(int i=0;i<Container->Points_List.length();i++)
    {
        //qDebug()<<Container.Points_List.at(i).Point.x()<<Container.Points_List.at(i).Point.y()<<endl;
        QPointF point(Container->Points_List.at(i).Point.x(),Container->Points_List.at(i).Point.y());
        points2.append(point);
    }

    //    QPointF p(250,250);
    //    points.append(p);
    for(int i=0;i<points2.count();i++)
    {
        QGraphicsEllipseItem *newPointCircle=
                new QGraphicsEllipseItem(points2[i].x()-1,points2[i].y()-1,2,2);
        newPointCircle->setBrush(Qt::red);

        shapes.append(newPointCircle);

    }
    showShape(shapes);
    shapes.clear();

}
void EditWidget::showpicture()
{
    QImage *Image =new QImage("D:/GIS_Project/123.jpg");
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(*Image));
                item->setScale(1);
                item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
                item->setPos(497000,3518000);
                this->addItem(item);


}
void EditWidget::mousePressClear(QGraphicsSceneMouseEvent *event)
{
    this->views()[0]->setCursor(Qt::PointingHandCursor);
    if(this->items().count()>0)
    {
        QGraphicsItem *selectItem =this->itemAt(event->scenePos(), this->items()[0]->transform());
        if(selectItem != NULL)
        {
            selectItem->setSelected(true);
            //this->removeItem(selectItem);
            this->clearSelection();
            Container->Delete_Point(selectItem->data(2).toInt());
         }
    }
}

EditWidget::~EditWidget()
{

}

