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
    ui->B_x->setText("497120");
    ui->B_y->setText("3518120");
    ui->A_x->setText("497220");
    ui->A_y->setText("3518220");
    ui->angle->setText("100");

    ui->alpha->setText("0");
    ui->Li->setText("120");
    ui->DZBi->setText("100");
    ui->Si->setText("58");
    ui->Xz->setText("497220");
    ui->Yz->setText("3518220");
}

pianxin::~pianxin()
{
    delete ui;
}

double pianxin::radiantoangle(double radian)
{
    double angle=radian*180/M_PI;
    return angle;
}

double pianxin::angletoradian(double angle)
{
    double radian=angle*M_PI/180;
    return radian;
}

QPointF pianxin::spincoordinate1(QPointF p,int n)//直角坐标系变为以A点为中心的点的坐标
{
    int a=497220;
    int b=3518220;//A点在原坐标系下的坐标。
//    qreal x1=497220-(p.x()*cos(n*M_PI/180)+p.y()*sin(n*M_PI/180));
//    qreal y1=3518220-(-p.x()*sin(n*M_PI/180)+p.y()*cos(n*M_PI/180));
    qreal x1=(p.x()-a)*cos(n*M_PI/180)+(p.y()-b)*sin(n*M_PI/180);
    qreal y1=-(p.x()-a)*sin(n*M_PI/180)+(p.y()-b)*cos(n*M_PI/180);
    QPointF p1(x1,y1);
    return p1;
}

QPointF pianxin::spincoordinate2(QPointF p, int n)//直角坐标系变回原坐标系
{
    int a=497220;
    int b=3518220;//A点在原坐标系下的坐标。
    qreal x1=a+(p.x()*cos(n*M_PI/180)-p.y()*sin(n*M_PI/180));
    qreal y1=b+(p.x()*sin(n*M_PI/180)+p.y()*cos(n*M_PI/180));
    QPointF p1(x1,y1);
    return p1;
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

//void pianxin::on_pianxin_OK_clicked()
//{
//    double s1=ui->s1->text().toDouble();
//    double s2=ui->s2->text().toDouble();
//    double d1=ui->d1->text().toDouble();
//    double d2=ui->d2->text().toDouble();
//    QPointF A(ui->A_x->text().toDouble(),ui->A_y->text().toDouble());
////    QPointF B(ui->B_x->text().toDouble(),ui->B_y->text().toDouble());
//    double angle=ui->angle->text().toDouble();

//    double s=sqrt((s2*(pow(s1,2)-pow(d1,2))-s1*(pow(s2,2)-pow(d2,2)))/(s1-s2));
//    double beita=acos((pow(s,2)+pow(s2,2)-pow(d2,2))/(2*s*s2))/M_PI*180;//C++中的三角函数操作的是弧度而非角度
//    qreal x0=A.x();
//    qreal y0=A.y();
////偏心点C D：
//    qreal xc=x0+s1*cos(angle*M_PI/180);
//    qreal yc=y0+s1*sin(angle*M_PI/180);
//    qreal xd=x0+s2*cos(angle*M_PI/180);
//    qreal yd=y0+s2*sin(angle*M_PI/180);
//    QGraphicsEllipseItem *cur_c=new QGraphicsEllipseItem(xc-5,yc-5,10,10);
//    cur_c->setBrush(Qt::blue);
//    area->addItem(cur_c);
//    QGraphicsEllipseItem *cur_d=new QGraphicsEllipseItem(xd-5,yd-5,10,10);
//    cur_d->setBrush(Qt::blue);
//    area->addItem(cur_d);

////测站：
//    QGraphicsEllipseItem *cur_a=new QGraphicsEllipseItem(A.x()-5,A.y()-5,10,10);
//    cur_a->setBrush(Qt::green);
//    area->addItem(cur_a);
//    qreal xp,yp;
//    bool isleft= ui->radioButton_left->isChecked();
//    if(isleft)
//    {
//        qDebug()<<"left"<<s<<cos((angle-beita)*M_PI/180)<<s*cos((angle-beita)*M_PI/180);
//        xp=x0+s*cos((angle-beita)*M_PI/180);
//        yp=y0+s*sin((angle-beita)*M_PI/180);
//    }
//    else
//    {
//        qDebug()<<"right"<<s<<cos((angle+beita)*M_PI/180)<<s*cos((angle+beita)*M_PI/180);
//        xp=x0+s*cos((angle+beita)*M_PI/180);
//        yp=y0+s*sin((angle+beita)*M_PI/180);
//    }


//    QPointF p(xp,yp);
//    qDebug()<<p;
//    QGraphicsEllipseItem *cur=new QGraphicsEllipseItem(p.x()-5,p.y()-5,10,10);
//    cur->setBrush(Qt::red);
//    area->addItem(cur);
//    this->close();
//}
void pianxin::on_pianxin_OK_clicked()
{
    double s1=ui->s1->text().toDouble();
    double s2=ui->s2->text().toDouble();
    double d1=ui->d1->text().toDouble();
    double d2=ui->d2->text().toDouble();
    QPointF B(ui->B_x->text().toDouble(),ui->B_y->text().toDouble());
    QPointF A(ui->A_x->text().toDouble(),ui->A_y->text().toDouble());
    double angle=ui->angle->text().toDouble();

    qDebug()<<"B"<<B<<atan(abs(B.y()/B.x()))*180/M_PI;
    qDebug()<<"pspin"<<spincoordinate1(B,atan(abs(B.y()/B.x()))*180/M_PI);

    qDebug()<<"B"<<B<<atan(abs(B.y()/B.x()))*180/M_PI;
    qDebug()<<"pspin"<<spincoordinate2(B,atan(abs(B.y()/B.x()))*180/M_PI);

    double s=sqrt((s2*(pow(s1,2)-pow(d1,2))-s1*(pow(s2,2)-pow(d2,2)))/(s1-s2));
    double beita=acos((pow(s,2)+pow(s2,2)-pow(d2,2))/(2*s*s2))/M_PI*180;//C++中的三角函数操作的是弧度而非角度
    qreal x0=A.x();
    qreal y0=A.y();
//后视点:
    QGraphicsEllipseItem *cur_b=new QGraphicsEllipseItem(B.x()-5,B.y()-5,10,10);
    cur_b->setBrush(Qt::yellow);
    area->addItem(cur_b);
//偏心点C D：
    qreal xc=x0+s1*cos(angle*M_PI/180);
    qreal yc=y0+s1*sin(angle*M_PI/180);
    qreal xd=x0+s2*cos(angle*M_PI/180);
    qreal yd=y0+s2*sin(angle*M_PI/180);

    QGraphicsEllipseItem *cur_c=new QGraphicsEllipseItem(xc-5,yc-5,10,10);
    cur_c->setBrush(Qt::blue);
    area->addItem(cur_c);
    QGraphicsEllipseItem *cur_d=new QGraphicsEllipseItem(xd-5,yd-5,10,10);
    cur_d->setBrush(Qt::blue);
    area->addItem(cur_d);

//测站：
    QGraphicsEllipseItem *cur_a=new QGraphicsEllipseItem(A.x()-5,A.y()-5,10,10);
    cur_a->setBrush(Qt::green);
    area->addItem(cur_a);
    qreal xp,yp;
    bool isleft= ui->radioButton_left->isChecked();
    if(isleft)
    {
        qDebug()<<"left"<<s<<cos((angle-beita)*M_PI/180)<<s*cos((angle-beita)*M_PI/180);
        xp=x0+s*cos((angle-beita)*M_PI/180);
        yp=y0+s*sin((angle-beita)*M_PI/180);
    }
    else
    {
        qDebug()<<"right"<<s<<cos((angle+beita)*M_PI/180)<<s*cos((angle+beita)*M_PI/180);
        xp=x0+s*cos((angle+beita)*M_PI/180);
        yp=y0+s*sin((angle+beita)*M_PI/180);
    }


    QPointF p(xp,yp);
    qDebug()<<"p"<<p;
    qDebug()<<"pspin"<<spincoordinate2(p,90);
    QGraphicsEllipseItem *cur=new QGraphicsEllipseItem(p.x()-5,p.y()-5,10,10);
    cur->setBrush(Qt::red);
    area->addItem(cur);
    this->close();
}
qreal pianxin::regulate(qreal q)
{
    return 3518220-(q-3518220);
}

void pianxin::on_OK_clicked()
{
    double Li=ui->Li->text().toDouble()*M_PI/180;//角度转为弧度
    double DZBi=ui->DZBi->text().toDouble();
    double Si=ui->Si->text().toDouble();
    double alpha=ui->alpha->text().toDouble()*M_PI/180;
    double Xz=ui->Xz->text().toDouble();
    double Yz=ui->Yz->text().toDouble();
    QGraphicsEllipseItem *cur_z=new QGraphicsEllipseItem(Xz-5,regulate(Yz-5),10,10);
    cur_z->setBrush(Qt::red);
    area->addItem(cur_z);

    int type=0;//初始化为上方
    /*
    type代表偏心点在待测点的方位
    0   下方
    1   上方
    2   右方
    3   左方
    */
    if(ui->radioButton_down_2->isChecked())
        type=1;
    else if(ui->radioButton_up_2->isChecked())
        type=0;
    else if(ui->radioButton_left_2->isChecked())
        type=2;
    else
        type=3;

    switch (type) {
    case 1://上
    {
        double alphaZB=alpha+Li;

        qreal Xbi=Xz+DZBi*cos(alphaZB);
        qreal Ybi=Yz+DZBi*sin(alphaZB);
        QGraphicsEllipseItem *cur_bi=new QGraphicsEllipseItem(Xbi-5,regulate(Ybi-5),10,10);
        cur_bi->setBrush(Qt::yellow);
        area->addItem(cur_bi);


        qreal Xb=Xz+(DZBi+Si)*cos(alphaZB);
        qreal Yb=Yz+(DZBi+Si)*sin(alphaZB);
        QGraphicsEllipseItem *cur_b=new QGraphicsEllipseItem(Xb-5,regulate(Yb-5),10,10);
        cur_b->setBrush(Qt::blue);
        area->addItem(cur_b);

        break;
    }
    case 0://下
    {
        double alphaZB=alpha+Li;

        qreal Xbi=Xz+DZBi*cos(alphaZB);
        qreal Ybi=Yz+DZBi*sin(alphaZB);
        QGraphicsEllipseItem *cur_bi=new QGraphicsEllipseItem(Xbi-5,regulate(Ybi-5),10,10);
        cur_bi->setBrush(Qt::yellow);//偏心点
        area->addItem(cur_bi);


        qreal Xb=Xz+(DZBi-Si)*cos(alphaZB);
        qreal Yb=Yz+(DZBi-Si)*sin(alphaZB);
        QGraphicsEllipseItem *cur_b=new QGraphicsEllipseItem(Xb-5,regulate(Yb-5),10,10);
        cur_b->setBrush(Qt::blue);//待测点
        area->addItem(cur_b);
        break;
    }
    case 3://左
    {
        double Dzb=sqrt(pow(DZBi,2)+pow(Si,2));//Z,B的距离
        double dalpha=atan(Si/DZBi);    //角BZBi(弧度)
        double alphaZB=alpha+Li+dalpha;//ZB与定向方向的角度


        double alpha0=alpha+Li;
        qreal Xbi=Xz+DZBi*cos(alpha0);
        qreal Ybi=Yz+DZBi*sin(alpha0);
        QGraphicsEllipseItem *cur_bi=new QGraphicsEllipseItem(Xbi-5,regulate(Ybi-5),10,10);
        cur_bi->setBrush(Qt::yellow);
        area->addItem(cur_bi);


        qreal Xb=Xz+Dzb*cos(alphaZB);
        qreal Yb=Yz+Dzb*sin(alphaZB);
        QGraphicsEllipseItem *cur_b=new QGraphicsEllipseItem(Xb-5,regulate(Yb-5),10,10);
        cur_b->setBrush(Qt::blue);
        area->addItem(cur_b);
        break;
    }
    case 2://右
    {
        double Dzb=sqrt(pow(DZBi,2)+pow(Si,2));//Z,B的距离
        double dalpha=atan(Si/DZBi);    //角BZBi(弧度)
        double alphaZB=alpha+Li-dalpha;//ZB与定向方向的角度


        double alpha0=alpha+Li;
        qreal Xbi=Xz+DZBi*cos(alpha0);
        qreal Ybi=Yz+DZBi*sin(alpha0);
        QGraphicsEllipseItem *cur_bi=new QGraphicsEllipseItem(Xbi-5,regulate(Ybi-5),10,10);
        cur_bi->setBrush(Qt::yellow);
        area->addItem(cur_bi);


        qreal Xb=Xz+Dzb*cos(alphaZB);
        qreal Yb=Yz+Dzb*sin(alphaZB);
        QGraphicsEllipseItem *cur_b=new QGraphicsEllipseItem(Xb-5,regulate(Yb-5),10,10);
        cur_b->setBrush(Qt::blue);
        area->addItem(cur_b);
        break;
    }
    default:
        break;
    }

}
