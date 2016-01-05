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
private slots:
    void on_pushButton_2_clicked();

    void on_pianxin_OK_clicked();

private:
    Ui::pianxin *ui;
};

#endif // PIANXIN_H
