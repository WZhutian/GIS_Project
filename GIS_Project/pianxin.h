#ifndef PIANXIN_H
#define PIANXIN_H

#include <QDialog>
#include<editwidget.h>
namespace Ui {
class pianxin;
}

class pianxin : public QDialog
{
    Q_OBJECT

public:
    explicit pianxin(QWidget *parent = 0);
    EditWidget *area;
    ~pianxin();
    void Get_Area(EditWidget &Area_out){
        area=&Area_out;
    }
    double radiantoangle(double radian);//弧度转为角度
    double angletoradian(double angle);//角度转为弧度
    QPointF spincoordinate1(QPointF p,int n);//直角坐标系变为以A点为中心的点的坐标
    QPointF spincoordinate2(QPointF p,int n);//直角坐标系变回原坐标系
    qreal regulate(qreal q);
private slots:
    void on_pushButton_2_clicked();

    void on_pianxin_OK_clicked();

    void on_OK_clicked();

private:
    Ui::pianxin *ui;
};

#endif // PIANXIN_H
