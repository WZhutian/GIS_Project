#ifndef PIANXIN_H
#define PIANXIN_H

#include <QDialog>

namespace Ui {
class pianxin;
}

class pianxin : public QDialog
{
    Q_OBJECT

public:
    explicit pianxin(QWidget *parent = 0);
    ~pianxin();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::pianxin *ui;
};

#endif // PIANXIN_H
