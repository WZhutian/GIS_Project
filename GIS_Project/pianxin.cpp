#include "pianxin.h"
#include "ui_pianxin.h"

pianxin::pianxin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pianxin)
{
    ui->setupUi(this);
    ui->s1->setText("150");
    ui->s2->setText("100");
    ui->d1->setText("100");
    ui->d2->setText("100");
    ui->A_x->setText("497220");
    ui->A_y->setText("3518220");
    ui->angle->setText("100");
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
    QGraphicsEllipseItem *cur=new QGraphicsEllipseItem(area->points3[FLWindex].x()+X-5,area->points3[FLWindex].y()-Y-5,10,10);
    cur->setBrush(Qt::red);
    area->addItem(cur);
    this->close();
}

void pianxin::on_pianxin_OK_clicked()
{
    double s1=ui->s1->text().toDouble();
    double s2=ui->s2->text().toDouble();
    double d1=ui->d1->text().toDouble();
    double d2=ui->d2->text().toDouble();
    QPointF A(ui->A_x->text().toDouble(),ui->A_y->text().toDouble());
    double angle=ui->angle->text().toDouble();

    double s=sqrt((s2*(pow(s1,2)-pow(d1,2))-s1*(pow(s2,2)-pow(d2,2)))/(s1-s2));
    double beita=acos((pow(s,2)+pow(s2,2)-pow(d2,2))/(2*s*s2));
    qreal x0=A.x();
    qreal y0=A.y();

//测站：
    QGraphicsEllipseItem *cur_a=new QGraphicsEllipseItem(A.x()-5,A.y()-5,10,10);
    cur_a->setBrush(Qt::green);
    area->addItem(cur_a);
    qreal xp,yp;
    bool isleft= ui->radioButton_left->isChecked();
    if(isleft)
    {
        xp=x0+s*cos(angle-beita);
        yp=y0+s*sin(angle-beita);
    }
    else
    {
        xp=x0+s*cos(angle+beita);
        yp=y0+s*sin(angle+beita);
    }
    QPointF p(xp,yp);
    qDebug()<<p;
    QGraphicsEllipseItem *cur=new QGraphicsEllipseItem(p.x()-5,p.y()-5,10,10);
    cur->setBrush(Qt::red);
    area->addItem(cur);
    this->close();
}
