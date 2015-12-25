#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editwidget.h"
#include <QtNetwork>
#include "container_list.h"
#include <QFileDialog>
#include <QStandardItemModel>
#include <gdal_readfile.h>
#include "tcpserver.h"
#include<QMouseEvent>
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
    void Get_TcpServer(TcpServer &TcpSever_Out);
    TcpServer *ser;
    QByteArray  intToByte(int i);
    int bytesToInt(QByteArray bytes);
    QModelIndex  init_modeIndex;
    //显示树形图
    QStandardItemModel *goodsModel;
    void Show_TreeView();
    int Change_Style_ID;//改变layer中的图元样式，索引值
    QColor Temp_Color_Pen;
    QColor Temp_Color_Brush;
    bool willtoRead;
private slots:
    //绘图部分
    void on_penStyleComboBox_currentIndexChanged(const QString penStyle);
    void on_penColorToolButton_clicked();
    void on_brushColorToolButton_clicked();
    void on_penWidthSpinBox_valueChanged(int penWidth);
    void on_action_Move_triggered();
    void on_action_Edit_triggered();
    void on_action_Draw_triggered();
    void on_action_ZoomIn_triggered();
    void on_action_ZoomOut_triggered();
    void on_action_ReadShp_triggered();
    void on_action_Refresh_triggered();
    //图层widget选框被点击
    void treeItemChanged ( QStandardItem * item );
    void slotCustomContextMenu(const QPoint &);

    void Show_Attr();
    void Change_Style();
    //socket部分,客户端client
    void ReadError(QAbstractSocket::SocketError);
    void on_action_Tcp_Connect_triggered();
    void on_action_Tcp_Sent_triggered();
    void on_action_Tcp_Time_triggered();

    void on_action_Tcp_Server_triggered();

    void on_action_movescene_triggered();
    void on_action_Start_Edit_triggered();

    void on_action_End_Edit_triggered();

    void on_action_Create_PointLayer_triggered();

    void on_action_Create_LineLayer_triggered();

    void on_action_Create_PolygenLayer_triggered();

    void on_Save_Style_clicked();

    void on_action_Show_LayerList_triggered();

protected:
//    void mousePressEvent(QMouseEvent *);
//    void mouseMoveEvent(QMouseEvent *);
private:
    Ui::MainWindow *ui;
    //socket部分，客户端client
    QTcpSocket * tcpClient;
    QTimer tm;
    QStringList list;
};

#endif // MAINWINDOW_H
