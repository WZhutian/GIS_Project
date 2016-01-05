#include "pianxin.h"
#include "ui_pianxin.h"

pianxin::pianxin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pianxin)
{
    ui->setupUi(this);
}

pianxin::~pianxin()
{
    delete ui;
}

void pianxin::on_pushButton_2_clicked()
{
    //直角坐标
    int FLWindex = ui->lineEdit_FLWindex->text().toInt();
    qreal X=ui->lineEdit_zj_X->text().toFloat();
    qreal Y=ui->lineEdit_zj_Y->text().toFloat();
    if(FLWindex<5)
        FLWindex+=4;
    else if(FLWindex<9)
        FLWindex+=5;
    else if(FLWindex<13)
        FLWindex+=6;
    else
        FLWindex+=7;
    QGraphicsEllipseItem *cur=new QGraphicsEllipseItem(area->points3[FLWindex].x()+X-5,area->points3[FLWindex].y()+Y-5,10,10);
    cur->setBrush(Qt::red);
    area->addItem(cur);
     this->close();
}
