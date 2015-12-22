﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editwidget.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QColorDialog>
#include <QMessageBox>
#include "mydatastream.h"
#include <QInputDialog>
QByteArray  MainWindow::intToByte(int i)
{
    QByteArray abyte0;
    abyte0.resize(4);
    abyte0[0] = (uchar)  (0x000000ff & i);
    abyte0[1] = (uchar) ((0x0000ff00 & i) >> 8);
    abyte0[2] = (uchar) ((0x00ff0000 & i) >> 16);
    abyte0[3] = (uchar) ((0xff000000 & i) >> 24);
    return abyte0;
}
int MainWindow::bytesToInt(QByteArray bytes) {
    int addr = bytes[0] & 0x000000FF;
    addr |= ((bytes[1] << 8) & 0x0000FF00);
    addr |= ((bytes[2] << 16) & 0x00FF0000);
    addr |= ((bytes[3] << 24) & 0xFF000000);
    return addr;
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    area=new EditWidget(this);

    QGraphicsView *view = new QGraphicsView(area,this);
    area->setSceneRect(497000,3518000,3500,3000);
    view->setMouseTracking(true);
    setCentralWidget(view);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setScene(area);
    view->show();
    //socket部分，客户端client
    tcpClient = new QTcpSocket(this);
    ui->action_Tcp_Sent->setEnabled(false);
    this->ui->action_Tcp_Time->setEnabled(false);
    //开始读取
    tcpClient->abort();
    connect(tcpClient,&QTcpSocket::readyRead,
            [&](){
        QByteArray Byte_size;
        QDataStream Message(this->tcpClient->readAll());
        Message>>Byte_size;
        int size=bytesToInt(Byte_size);
        //将客户端的修改内容添加到服务器的容器中，并对相应图元进行删除操作
        for(int i=0;i<size;i++){
            St_Layers Layers_out;
            Message>>Layers_out;
            //对发来的layers做分析，先分析layer中操作，对容器进行修改，存在的都删除；
            for(int t=0;t<Layers_out.PC_ID.size();t++){
                if(Layers_out.Change_Way.at(t)!=2){
                    int index=Container->Current_search(Layers_out.Layer_ID,Layers_out.PC_ID.at(t),Layers_out.Index_Part.at(t),Layers_out.Ob_Type);
                    if(Layers_out.Ob_Type==0){
                        qDebug()<<"remmoved Point:";
                        qDebug()<<Container->Points_List.at(index).Point;
                        Container->Points_List.removeAt(index);
                    }else if(Layers_out.Ob_Type==1){
                        Container->Lines_List.removeAt(index);
                    }else{
                        Container->Polygens_List.removeAt(index);
                    }
                }
            }
        }
        //Step3:将数据包添加到本地容器中去
        //按顺序添加后面的包
        QByteArray Pt_size;
        Message>>Pt_size;
        int size_Pt=bytesToInt(Pt_size);
        for(int j=0;j<size_Pt;j++){
            St_Points Points_out;
            Message>>Points_out;
            //添加到本地容器
            int insert_index=Container->Current_insert(Points_out.Layer_ID,Points_out.PC_ID,Points_out.Index_Part,0);
            Container->Points_List.insert(insert_index,Points_out);
        }
        QByteArray Ln_size;
        Message>>Ln_size;
        int size_Ln=bytesToInt(Ln_size);
        for(int j=0;j<size_Ln;j++){
            St_Lines Lines_out;
            Message>>Lines_out;
            //添加到本地容器
            int insert_index=Container->Current_insert(Lines_out.Layer_ID,Lines_out.PC_ID,Lines_out.Index_Part,1);
            Container->Lines_List.insert(insert_index,Lines_out);
        }
        QByteArray Pl_size;
        Message>>Pl_size;
        int size_Pl=bytesToInt(Pl_size);
        for(int j=0;j<size_Pl;j++){
            St_Polygens Polygens_out;
            Message>>Polygens_out;
            //添加到本地容器
            int insert_index=Container->Current_insert(Polygens_out.Layer_ID,Polygens_out.PC_ID,Polygens_out.Index_Part,2);
            Container->Polygens_List.insert(insert_index,Polygens_out);
        }
    });
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ReadError(QAbstractSocket::SocketError)));//错误信号
    connect(&tm,&QTimer::timeout,[&](){
        this->on_action_Tcp_Sent_triggered();
    });//循环
    connect(tcpClient,&QTcpSocket::disconnected,[](){qDebug()<< "断开连接" ;});
    portd="6666";
    h_time=60;
}

void MainWindow::Get_Container(Container_List &Container_Out){
    Container=&Container_Out;
    area->Get_Container(*Container);
}

void MainWindow::Get_TcpServer(TcpServer &TcpSever_Out){
    ser=&TcpSever_Out;
}
MainWindow::~MainWindow()
{
    delete ui;
    delete tcpClient;
}
//绘图部分------------------------------------------------------------
void MainWindow::on_penStyleComboBox_currentIndexChanged(const QString penStyle)
{
    if(penStyle == QStringLiteral("实线"))
    {
        area->setPenStyle(Qt::SolidLine);   //利用PaintArea类中的枚举变量
    }
    else if(penStyle == QStringLiteral("虚线"))
    {
        area->setPenStyle(Qt::DotLine);
    }
}
void MainWindow::on_shapeComboBox_currentIndexChanged(const QString shape)
{
    if(shape == QStringLiteral("点"))
    {
        area->setShape(EditWidget::PointType);
    }
    else if(shape == QStringLiteral("线"))
    {
        area->setShape(EditWidget::PolylineType);
    }
    else if(shape == QStringLiteral("面"))
    {
        area->setShape(EditWidget::PolygonType);
    }
}

void MainWindow::on_penWidthSpinBox_valueChanged(int penWidth)
{
    area->setPenWidth(penWidth);
}




void MainWindow::on_penColorToolButton_clicked()
{
    QColor newColor = QColorDialog::getColor();  //从颜色对话框获得颜色
    if (newColor.isValid())   //如果得到的是可用的颜色
    {
        area->setPenColor(newColor);
        QPalette palette = ui->textBrowser_Pen->palette(); //显示这个颜色
        palette.setColor(QPalette::Base,newColor);
        ui->textBrowser_Pen->setPalette(palette);
    }
}

void MainWindow::on_brushColorToolButton_clicked()
{
    QColor newColor = QColorDialog::getColor();  //从颜色对话框获得颜色
    if (newColor.isValid())   //如果得到的是可用的颜色
    {
        area->setBrushColor(newColor);
        QPalette palette = ui->textBrowser_Brush->palette(); //显示这个颜色
        palette.setColor(QPalette::Base,newColor);
        ui->textBrowser_Brush->setPalette(palette);
    }
}

void MainWindow::on_action_Draw_triggered()
{
    if( ui->action_Draw->isChecked()==true)
    {
        area->views()[0]->setCursor(Qt::CrossCursor);
        area->setState(EditWidget::DrawType);
        ui->action_Move->setChecked(false);
        ui->action_Edit->setChecked(false);
        ui->action_Refresh->setChecked(false);
        ui->action_movescene->setChecked(false);
    }
}

void MainWindow::on_action_Move_triggered()
{
    if( ui->action_Move->isChecked()==true)
    {
        area->views()[0]->setCursor(Qt::OpenHandCursor);
        area->setState(EditWidget::MoveType);
        ui->action_Draw->setChecked(false);
        ui->action_Edit->setChecked(false);
        ui->action_Refresh->setChecked(false);
        ui->action_movescene->setChecked(false);
    }
}

void MainWindow::on_action_Edit_triggered()
{
    if( ui->action_Edit->isChecked()==true)
    {
        area->views()[0]->setCursor(Qt::PointingHandCursor);
        area->setState(EditWidget::EditType);
        ui->action_Move->setChecked(false);
        ui->action_Draw->setChecked(false);
        ui->action_Refresh->setChecked(false);
        ui->action_movescene->setChecked(false);
    }
}



void MainWindow::on_action_Refresh_triggered()
{
    if(ui->action_Refresh->isChecked()==true)
    {
        area->views()[0]->setCursor(Qt::PointingHandCursor);
        area->setState(EditWidget::clearType);
        ui->action_Move->setChecked(false);
        ui->action_Draw->setChecked(false);
        ui->action_Edit->setChecked(false);
        ui->action_movescene->setChecked(false);
    }

}
void MainWindow::on_action_movescene_triggered()
{
    if(ui->action_movescene->isChecked()==true)
    {
        area->views()[0]->setCursor(Qt::OpenHandCursor);
        area->setState(EditWidget::MoveSceneType);
        ui->action_Draw->setChecked(false);
        ui->action_Edit->setChecked(false);
        ui->action_Move->setChecked(false);
        ui->action_Refresh->setChecked(false);
    }
}


void MainWindow::on_action_ZoomIn_triggered()
{
    area->views()[0]->scale(1.2,1.2);
}

void MainWindow::on_action_ZoomOut_triggered()
{
    area->views()[0]->scale(1/1.2,1/1.2);
}


//tcp部分------------------------------------------------------------



void MainWindow::ReadError(QAbstractSocket::SocketError)//出错
{
    tcpClient->disconnectFromHost();
    ui->action_Tcp_Connect->setText("连接");
    qDebug()<<tr("连接出错：%1").arg(tcpClient->errorString());
    ui->action_Tcp_Sent->setEnabled(false);
    tm.stop();
    this->ui->action_Tcp_Time->setEnabled(false);
    this->ui->action_Tcp_Time->setText("启动定时");
}

void MainWindow::on_action_Tcp_Connect_triggered()
{
    qDebug() << "点击连接：" ;
    if ("连接" == this->ui->action_Tcp_Connect->text())
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("请输入对方IP地址："), QLineEdit::Normal,
                                             "", &ok);
        if (ok && !text.isEmpty()){
            ipAdd=text;//填入IP地址
            tcpClient->connectToHost(ipAdd,portd.toInt());
            if (tcpClient->waitForConnected(1000))
            {
                ui->action_Tcp_Connect->setText("断开");
                qDebug()<<"连接服务器成功";
                ui->action_Tcp_Sent->setEnabled(true);
                this->ui->action_Tcp_Time->setEnabled(true);
                QByteArray block;//保存要发送的二进制数据
                QDataStream out (&block,QIODevice::WriteOnly);//输出流
                int PCid_size=0;
                out<<intToByte(PCid_size);
                int layer_size=0;
                out<<intToByte(layer_size);
                tcpClient->write(block);
            }
        }
    }
    else
    {
        tcpClient->disconnectFromHost();
        if (tcpClient->state() == QAbstractSocket::UnconnectedState || tcpClient->waitForDisconnected(1000) )
        {
            ui->action_Tcp_Connect->setText("连接");
            qDebug()<<"断开服务器";
            ui->action_Tcp_Sent->setEnabled(false);
            tm.stop();
            this->ui->action_Tcp_Time->setEnabled(false);
            this->ui->action_Tcp_Time->setText("启动定时");
        }
        ipAdd="";//将IP地址清除
    }
}

void MainWindow::on_action_Tcp_Sent_triggered()
{
    qDebug() << "点击发送：" ;
    QByteArray block;//保存要发送的二进制数据
    QDataStream out (&block,QIODevice::WriteOnly);
    int PC_id_sent=Container->PC_ID;
    out<<intToByte(PC_id_sent);
    int layer_size=Container->Layers_List.size();
    out<<intToByte(layer_size);
    int Point_Mod_size=0,Line_Mod_size=0,Polygen_Mod_size=0;//记录修改操作中的3种要素的数量
    for(int i=0;i<layer_size;i++){
        //统计各修改元素的数量
        if(Container->Layers_List.at(i).Ob_Type==0){
            Point_Mod_size+=Container->Layers_List.at(i).PC_ID.size();
        }else if(Container->Layers_List.at(i).Ob_Type==0){
            Line_Mod_size+=Container->Layers_List.at(i).PC_ID.size();
        }else{
            Polygen_Mod_size+=Container->Layers_List.at(i).PC_ID.size();
        }
        out<<Container->Layers_List.at(i);
    }
    //从图层中查找到点图层，收集修改信息，将修改的要素从对应点容器中提取出来放入输出流中
    out<<Point_Mod_size;
    for(int i=0;i<layer_size;i++){
        if(Container->Layers_List.at(i).Ob_Type==0){
            for(int j=0;j<Container->Layers_List.at(i).PC_ID.size();j++){
                int pc_id = Container->PC_ID;//获取自身PCID
                int index = Container->Layers_List.at(i).Index_Part.at(j);

                out<<Container->Points_List.at(Container->Current_search(Container->Layers_List.at(i).Layer_ID,pc_id,index,0));
            }
        }
    }
    out<<Line_Mod_size;
    for(int i=0;i<layer_size;i++){
        if(Container->Layers_List.at(i).Ob_Type==1){
            for(int j=0;j<Container->Layers_List.at(i).PC_ID.size();j++){
                int pc_id = Container->PC_ID;//获取自身PCID
                int index = Container->Layers_List.at(i).Index_Part.at(j);

                out<<Container->Lines_List.at(Container->Current_search(Container->Layers_List.at(i).Layer_ID,pc_id,index,1));

            }
        }
    }
    out<<Polygen_Mod_size;
    for(int i=0;i<layer_size;i++){
        if(Container->Layers_List.at(i).Ob_Type==2){
            for(int j=0;j<Container->Layers_List.at(i).PC_ID.size();j++){
                int pc_id = Container->PC_ID;//获取自身PCID
                int index = Container->Layers_List.at(i).Index_Part.at(j);
                out<<Container->Polygens_List.at(Container->Current_search(Container->Layers_List.at(i).Layer_ID,pc_id,index,2));
            }
        }
    }
    //清空本地通信判断
    for(int i=0;i<layer_size;i++){
        Container->Layers_List[i].PC_ID.clear();
        Container->Layers_List[i].Index_Part.clear();
        Container->Layers_List[i].Change_Way.clear();
    }
    if (block.isEmpty())
    {
        qDebug()<<"待发送数据为空！";
        return ;
    }
    tcpClient->write(block);
}

void MainWindow::on_action_Tcp_Time_triggered()
{
    if ("启动定时" == this->ui->action_Tcp_Time->text())
    {
        int h;
        h = h_time*1000;
        tm.start(h);
        this->ui->action_Tcp_Time->setText("停止定时");
    }
    else
    {
        tm.stop();
        this->ui->action_Tcp_Time->setText("启动定时");
    }
}

void MainWindow::on_action_ReadShp_triggered()//读shp文件
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Image Files(*.jpg *.png *.shp)"));
    if(path.right(4)==".shp"){
        GDAL_ReadFile test(path.toLatin1().data(),*Container);
        test.Get_Data();
    }else{
        St_Raster_images temp;
        temp.Image=new QImage(path);
        St_Layers temp_ly;
        temp_ly.Layer_ID=Container->Layers_List.size();
        temp_ly.Layer_Name="Image Layer";
        Container->Images_List.append(temp);
        Container->Layers_List.append(temp_ly);
    }
}
void MainWindow::Show_TreeView(){
    //清空
    //显示所有
}

void MainWindow::on_action_Tcp_Server_triggered()
{
    if ("启动服务" == this->ui->action_Tcp_Server->text())
    {
        ser->listen(QHostAddress::Any,6666);
        ui->action_Tcp_Server->setText("停止服务");
        ui->action_Tcp_Connect->setEnabled(false);
    }
    else
    {
        ui->action_Tcp_Server->setText("启动服务");
    }

    this->ui->action_Tcp_Server->setEnabled(false);

}

