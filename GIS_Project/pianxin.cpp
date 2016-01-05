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
}
