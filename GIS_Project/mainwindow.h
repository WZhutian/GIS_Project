#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editwidget.h"
#include <QtNetwork>
#include "container_list.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Container_List *Container;
    EditWidget *area;
    ~MainWindow();
    QString ipAdd;//服务器IP地址
    QString portd;//服务器端口号
    int h_time;//定时器时间
    void Get_Container(Container_List &Container_Out);
private slots:
    //绘图部分
    void on_penStyleComboBox_currentIndexChanged(const QString penStyle);
    void on_shapeComboBox_currentIndexChanged(const QString shape);
    void on_penColorToolButton_clicked();
    void on_brushColorToolButton_clicked();
    void on_penWidthSpinBox_valueChanged(int penWidth);
    void on_action_Move_triggered();
    void on_action_Edit_triggered();
    void on_action_Draw_triggered();
    void on_action_Clear_triggered();
    void on_action_ZoomIn_triggered();
    void on_action_ZoomOut_triggered();
    void on_drawpoints_clicked();
    //socket部分,客户端client
    void ReadError(QAbstractSocket::SocketError);

    void on_pushSent_clicked();
    void on_pushConnect_clicked();
    void on_timeBut_clicked();

private:
    Ui::MainWindow *ui;
    //socket部分，客户端client
    QTcpSocket * tcpClient;
    QTimer tm;
    QStringList list;
};

#endif // MAINWINDOW_H
