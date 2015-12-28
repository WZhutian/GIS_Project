#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editwidget.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QColorDialog>
#include <QMessageBox>
#include "mydatastream.h"
#include <QInputDialog>
#include <QBrush>
#include <QTableWidget>
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
    willtoRead=false;
    ui->action_End_Edit->setEnabled(false);
    //画板
    Change_Style_ID=-1;
    Temp_Color_Brush=Qt::white;
    Temp_Color_Pen=Qt::black;
    ui->dockWidget->hide();
    init_modeIndex = ui->treeView->currentIndex();
    //显示表头
    goodsModel = new QStandardItemModel(0, 3,this);
    ui->treeView->setModel(goodsModel);
    ui->treeView->setColumnWidth(0,43);
    ui->treeView->setColumnWidth(1,130);
    ui->treeView->setColumnWidth(2,80);
    goodsModel->setHeaderData(0, Qt::Horizontal,tr("显示"));
    goodsModel->setHeaderData(1, Qt::Horizontal, tr("图层名"));
    goodsModel->setHeaderData(2, Qt::Horizontal, tr("图层类型"));
    ui->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(goodsModel,&QStandardItemModel::itemChanged, this ,&MainWindow::treeItemChanged );

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(slotCustomContextMenu(const QPoint &)));
    //绘图部分，scene初始化
    area=new EditWidget(this);
    QGraphicsView *view = new QGraphicsView(area,this);
    area->Get_Graphicview(*view);
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
        if(willtoRead==true){
            QByteArray Byte_size;
            QDataStream Message(this->tcpClient->readAll());
            Message>>Byte_size;
            int size=bytesToInt(Byte_size);
            if(Container->Layers_List.size()==0){//首次连接的情况
                for(int i=0;i<size;i++){
                    St_Layers Layers_out;
                    Message>>Layers_out;
                    //初始化
                    Container->PC_ID=Layers_out.Pc_numbers+1;
                    Layers_out.Size=0;
                    for(int j=0;j<10;j++){
                        Layers_out.Every_size[j]=0;
                    }
                    Layers_out.PC_ID.clear();
                    Layers_out.Index_Part.clear();
                    Layers_out.Change_Way.clear();
                    //
                    Container->Layers_List.append(Layers_out);
                    St_Items temp_item;
                    Container->Items_List.append(temp_item);
                    Show_TreeView();
                }
                //Step3:将数据包添加到本地容器中去
                //按顺序添加后面的包
                QByteArray Pt_size;
                Message>>Pt_size;
                int size_Pt=bytesToInt(Pt_size);
                qDebug()<<size_Pt;
                for(int j=0;j<size_Pt;j++){
                    St_Points Points_out;
                    Message>>Points_out;
                    //添加到本地容器
                    Container->Points_List.append(Points_out);

                    int tempPcID=Container->PC_ID;
                    int tempLayerID=Container->Layer_ID;
                    Container->Layer_ID=Points_out.Layer_ID;
                    Container->PC_ID=Points_out.PC_ID;
                    Container->Add_Point_Item(Points_out.Point,Points_out.Index_Part);
                    Container->PC_ID=tempPcID;
                    Container->Layer_ID=tempLayerID;

                    QGraphicsEllipseItem *newPointCircle=
                            new QGraphicsEllipseItem(Points_out.Point.x()-5,Points_out.Point.y()-5,10,10);
                    // newPointCircle->setBrush(Qt::red);
                    newPointCircle->setFlag(QGraphicsItem::ItemIsMovable,false);
                    newPointCircle->setFlag(QGraphicsItem::ItemIsSelectable,false);
                    newPointCircle->setData(0,Points_out.PC_ID);
                    newPointCircle->setData(1,Points_out.Layer_ID);
                    newPointCircle->setData(2,Points_out.Index_Part);
                    Container->Items_List[Points_out.Layer_ID].Cur_Item.append(newPointCircle);
                    area->addItem(newPointCircle);
                }
                QByteArray Ln_size;
                Message>>Ln_size;
                int size_Ln=bytesToInt(Ln_size);
                for(int j=0;j<size_Ln;j++){
                    St_Lines Lines_out;
                    Message>>Lines_out;
                    //添加到本地容器
                    Container->Lines_List.append(Lines_out);

                    int tempPcID=Container->PC_ID;
                    int tempLayerID=Container->Layer_ID;
                    Container->Layer_ID=Lines_out.Layer_ID;
                    Container->PC_ID=Lines_out.PC_ID;
                    Container->Add_Line_Item(Lines_out.Line_FromTo,Lines_out.Index_Part);
                    Container->PC_ID=tempPcID;
                    Container->Layer_ID=tempLayerID;

                    QPainterPath *path=new QPainterPath(Lines_out.Line_FromTo[0]);
                    for(int i=1;i<Lines_out.Line_FromTo.count();++i)
                    {
                        path->lineTo(Lines_out.Line_FromTo[i]);
                    }
                    QGraphicsPathItem *cur=new QGraphicsPathItem();
                    cur->setPath(*path);
                    //cur->setPen(*pen);
                    cur->setFlag(QGraphicsItem::ItemIsMovable,false);
                    cur->setFlag(QGraphicsItem::ItemIsSelectable,false);
                    cur->setData(0,Lines_out.PC_ID);
                    cur->setData(1,Lines_out.Layer_ID);
                    cur->setData(2,Lines_out.Index_Part);
                    Container->Items_List[Lines_out.Layer_ID].Cur_Item.append(cur);
                    area->addItem(cur);
                }
                QByteArray Pl_size;
                Message>>Pl_size;
                int size_Pl=bytesToInt(Pl_size);
                for(int j=0;j<size_Pl;j++){
                    St_Polygens Polygens_out;
                    Message>>Polygens_out;
                    //添加到本地容器
                    Container->Polygens_List.append(Polygens_out);

                    int tempPcID=Container->PC_ID;
                    int tempLayerID=Container->Layer_ID;
                    Container->Layer_ID=Polygens_out.Layer_ID;
                    Container->PC_ID=Polygens_out.PC_ID;
                    Container->Add_Polygen_Item(Polygens_out.Polygen_Round,Polygens_out.Index_Part);
                    Container->PC_ID=tempPcID;
                    Container->Layer_ID=tempLayerID;

                    QGraphicsPolygonItem *cur=new QGraphicsPolygonItem();
                    QVector<QPointF>Poly_Item_Out;
                    for(int i=0;i<Polygens_out.Polygen_Round.size();i++){
                        Poly_Item_Out.append(Polygens_out.Polygen_Round.at(i));
                    }
                    qDebug()<<Polygens_out.Polygen_Round.size();
                    qDebug()<<Poly_Item_Out.size();
                    QPolygonF *curPolygon=new QPolygonF(Poly_Item_Out);
                    cur->setPolygon(*curPolygon);
                    cur->setFlag(QGraphicsItem::ItemIsMovable,false);
                    cur->setFlag(QGraphicsItem::ItemIsSelectable,false);
                    //    cur->setPen(*pen);
                    //    cur->setBrush(brushColor);
                    cur->setData(0,Polygens_out.PC_ID);
                    cur->setData(1,Polygens_out.Layer_ID);
                    cur->setData(2,Polygens_out.Index_Part);
                    Container->Items_List[Polygens_out.Layer_ID].Cur_Item.append(cur);
                    area->addItem(cur);
                }
            }else{
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
                                //TODO删除判断
                                area->removeItem(area->itemAt(Container->Points_List.at(index).Point,area->items()[0]->transform()));
                                Container->Points_List.removeAt(index);
                            }else if(Layers_out.Ob_Type==1){
                                //TODO删除判断
                                area->removeItem( area->itemAt(Container->Lines_List.at(index).Line_FromTo.at(0),area->items()[0]->transform()));
                                Container->Lines_List.removeAt(index);
                            }else{
                                //TODO删除判断
                                area->removeItem( area->itemAt(Container->Polygens_List.at(index).Polygen_Round.at(0),area->items()[0]->transform()));
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
                qDebug()<<size_Pt;
                for(int j=0;j<size_Pt;j++){
                    St_Points Points_out;
                    Message>>Points_out;
                    //添加到本地容器
                    int insert_index=Container->Current_insert(Points_out.Layer_ID,Points_out.PC_ID,Points_out.Index_Part,0);
                    Container->Points_List.insert(insert_index,Points_out);
                    int tempPcID=Container->PC_ID;
                    int tempLayerID=Container->Layer_ID;
                    Container->Layer_ID=Points_out.Layer_ID;
                    Container->PC_ID=Points_out.PC_ID;
                    Container->Add_Point_Item(Points_out.Point,Points_out.Index_Part);
                    Container->PC_ID=tempPcID;
                    Container->Layer_ID=tempLayerID;

                    QGraphicsEllipseItem *newPointCircle=
                            new QGraphicsEllipseItem(Points_out.Point.x()-5,Points_out.Point.y()-5,10,10);
                    // newPointCircle->setBrush(Qt::red);
                    newPointCircle->setFlag(QGraphicsItem::ItemIsMovable,false);
                    newPointCircle->setFlag(QGraphicsItem::ItemIsSelectable,false);
                    newPointCircle->setData(0,Points_out.PC_ID);
                    newPointCircle->setData(1,Points_out.Layer_ID);
                    newPointCircle->setData(2,Points_out.Index_Part);
                    Container->Items_List[Points_out.Layer_ID].Cur_Item.append(newPointCircle);
                    area->addItem(newPointCircle);
                }
                QByteArray Ln_size;
                Message>>Ln_size;
                int size_Ln=bytesToInt(Ln_size);
                for(int j=0;j<size_Ln;j++){
                    St_Lines Lines_out;
                    Message>>Lines_out;
                    //添加到本地容器
                    qDebug()<<Lines_out.Line_FromTo.size();
                    int insert_index=Container->Current_insert(Lines_out.Layer_ID,Lines_out.PC_ID,Lines_out.Index_Part,1);
                    Container->Lines_List.insert(insert_index,Lines_out);
                    int tempPcID=Container->PC_ID;
                    int tempLayerID=Container->Layer_ID;
                    Container->Layer_ID=Lines_out.Layer_ID;
                    Container->PC_ID=Lines_out.PC_ID;
                    Container->Add_Line_Item(Lines_out.Line_FromTo,Lines_out.Index_Part);
                    Container->PC_ID=tempPcID;
                    Container->Layer_ID=tempLayerID;

                    QPainterPath *path=new QPainterPath(Lines_out.Line_FromTo[0]);
                    for(int i=1;i<Lines_out.Line_FromTo.count();++i)
                    {
                        path->lineTo(Lines_out.Line_FromTo[i]);
                    }
                    QGraphicsPathItem *cur=new QGraphicsPathItem();
                    cur->setPath(*path);
                    //cur->setPen(*pen);
                    cur->setFlag(QGraphicsItem::ItemIsMovable,false);
                    cur->setFlag(QGraphicsItem::ItemIsSelectable,false);
                    cur->setData(0,Lines_out.PC_ID);
                    cur->setData(1,Lines_out.Layer_ID);
                    cur->setData(2,Lines_out.Index_Part);
                    Container->Items_List[Lines_out.Layer_ID].Cur_Item.append(cur);
                    area->addItem(cur);
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
                    int tempPcID=Container->PC_ID;
                    int tempLayerID=Container->Layer_ID;
                    Container->Layer_ID=Polygens_out.Layer_ID;
                    Container->PC_ID=Polygens_out.PC_ID;
                    Container->Add_Polygen_Item(Polygens_out.Polygen_Round,Polygens_out.Index_Part);
                    Container->PC_ID=tempPcID;
                    Container->Layer_ID=tempLayerID;

                    QGraphicsPolygonItem *cur=new QGraphicsPolygonItem();
                    QVector<QPointF>Poly_Item_Out;
                    for(int i=0;i<Polygens_out.Polygen_Round.size();i++){
                        Poly_Item_Out.append(Polygens_out.Polygen_Round.at(i));
                    }
                    qDebug()<<Polygens_out.Polygen_Round.size();
                    qDebug()<<Poly_Item_Out.size();
                    QPolygonF *curPolygon=new QPolygonF(Poly_Item_Out);
                    cur->setPolygon(*curPolygon);
                    cur->setFlag(QGraphicsItem::ItemIsMovable,false);
                    cur->setFlag(QGraphicsItem::ItemIsSelectable,false);
                    //    cur->setPen(*pen);
                    //    cur->setBrush(brushColor);
                    cur->setData(0,Polygens_out.PC_ID);
                    cur->setData(1,Polygens_out.Layer_ID);
                    cur->setData(2,Polygens_out.Index_Part);
                    Container->Items_List[Polygens_out.Layer_ID].Cur_Item.append(cur);
                    area->addItem(cur);
                }
            }
            willtoRead=false;
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
    database.Get_Container(*Container);
    database.Get_Area(*area);
    Show_TreeView();
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
        Container->setPenStyle(Qt::SolidLine);
    }
    else if(penStyle == QStringLiteral("虚线"))
    {
        area->setPenStyle(Qt::DotLine);
        Container->setPenStyle(Qt::DotLine);
    }
}
void MainWindow::on_penWidthSpinBox_valueChanged(int penWidth)
{
    area->setPenWidth(penWidth);
    Container->setPenWidth(penWidth);
}

void MainWindow::on_penColorToolButton_clicked()
{
    QColor newColor = QColorDialog::getColor();  //从颜色对话框获得颜色
    if (newColor.isValid())   //如果得到的是可用的颜色
    {
        area->setPenColor(newColor);
        Container->setPenColor(newColor);
        Temp_Color_Pen=newColor;
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
        Container->setBrushColor(newColor);
        Temp_Color_Brush=newColor;
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
        // ui->action_Move->setChecked(false);
        ui->action_Edit->setChecked(false);
        ui->action_Refresh->setChecked(false);
        ui->action_movescene->setChecked(false);
    }
}

//void MainWindow::on_action_Move_triggered()
//{
//    if( ui->action_Move->isChecked()==true)
//    {
//        area->views()[0]->setCursor(Qt::OpenHandCursor);
//        area->setState(EditWidget::MoveType);
//        ui->action_Draw->setChecked(false);
//        ui->action_Edit->setChecked(false);
//        ui->action_Refresh->setChecked(false);
//        ui->action_movescene->setChecked(false);
//    }
//}

void MainWindow::on_action_Edit_triggered()
{
    if( ui->action_Edit->isChecked()==true)
    {
        area->views()[0]->setCursor(Qt::PointingHandCursor);
        area->setState(EditWidget::EditType);
        //ui->action_Move->setChecked(false);
        ui->action_Draw->setChecked(false);
        ui->action_Refresh->setChecked(false);
        ui->action_movescene->setChecked(false);
        //        ui->action_Draw->setEnabled(false);
        //        ui->action_Refresh->setEnabled(false);
    }
}



void MainWindow::on_action_Refresh_triggered()
{
    if(ui->action_Refresh->isChecked()==true)
    {
        area->views()[0]->setCursor(Qt::PointingHandCursor);
        area->setState(EditWidget::clearType);
        //ui->action_Move->setChecked(false);
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
        //ui->action_Move->setChecked(false);
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
//////////////////////////TCP服务器///////////////////////
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
    ui->menu_4->setEnabled(false);

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
            willtoRead=true;
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
        willtoRead=false;
    }
    ui->menu_4->setEnabled(false);
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
    willtoRead=true;
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
///////////////////////////////读shpfile////////////////
void MainWindow::on_action_ReadShp_triggered()//读shp文件
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Image Files(*.jpg *.png *.shp)"));
    if(path.right(4)==".shp"){
        bool ok;
        QString text = QInputDialog::getText(this, tr("读取shpfile"),
                                             tr("请输入图层名："), QLineEdit::Normal,
                                             "New Shpfile Layer", &ok);
        if (ok && !text.isEmpty()){
            GDAL_ReadFile test(path.toLatin1().data(),*Container);
            test.Get_Data(text);
            qDebug()<<Container->Layers_List.at(0).Size;
            Show_TreeView();
            area->showShape(Container->Items_List[Container->Items_List.size()-1].Cur_Item,Container->Layers_List.at(Container->Layers_List.size()-1).Size);
        }
    }else if(path.right(4)==".jpg"||path.right(4)==".png"){

        qDebug()<<"inhere";
        bool ok;
        QString text = QInputDialog::getText(this, tr("读取图片"),
                                             tr("依次输入左上角XY坐标以及右下角XY坐标，以逗号分隔开:"), QLineEdit::Normal,
                                             "497000,3518000,497000,3518000", &ok);

        QStringList text_list=text.split(",");
        St_Raster_images temp;
        temp.Image=new QImage(path);

        temp.Image->scaledToWidth(text_list.at(2).toInt()-text_list.at(0).toInt());
        temp.Image->scaledToHeight(text_list.at(3).toInt()-text_list.at(1).toInt());
        if (ok && !text.isEmpty()){
            temp.Image_Name=text;
            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(*temp.Image));
            item->setScale(1);
            item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            item->setPos(text_list.at(0).toFloat(),text_list.at(1).toFloat());
            temp.image_Item=item;
            Container->Images_List.append(temp);
            area->addItem(Container->Images_List.at(Container->Images_List.size()-1).image_Item);
            Show_TreeView();
        }

    }
}
////////////////////////////////图层部分//////////////////////
void MainWindow::Show_TreeView(){
    //清空
    goodsModel->removeRows(0,goodsModel->rowCount());
    //显示所有
    for (int i = 0; i <  Container->Layers_List.size(); ++i)
    {

        QList<QStandardItem *> items;

        QStandardItem *item_1 = new QStandardItem();
        item_1->setCheckable(true);
        item_1->setCheckState(Qt::Checked);
        items.push_back(item_1);


        QStandardItem *item_2 = new QStandardItem(Container->Layers_List.at(i).Layer_Name);
        items.push_back(item_2);

        QString type;
        if(Container->Layers_List.at(i).Ob_Type==0){
            type="点";
        }else if(Container->Layers_List.at(i).Ob_Type==1){
            type="线";
        }else{
            type="面";
        }
        QStandardItem *item_3 = new QStandardItem(type);
        items.push_back(item_3);

        goodsModel->appendRow(items);
    }
    for(int i=0;i<Container->Images_List.size();i++){
        QList<QStandardItem *> items;
        QStandardItem *item_1 = new QStandardItem();
        item_1->setCheckState(Qt::Checked);
        item_1->setCheckable(true);
        items.push_back(item_1);
        QStandardItem *item_2 = new QStandardItem(Container->Images_List.at(i).Image_Name);
        items.push_back(item_2);
        QStandardItem *item_3 = new QStandardItem("图片");
        items.push_back(item_3);
        goodsModel->appendRow(items);
    }
}

void MainWindow :: treeItemChanged ( QStandardItem * item )
{
    int layer_id = item->row();
    //修改图层名,保护图层类型不变
    if(Container->Layers_List.size()<layer_id+1){
        Container->Images_List[layer_id-Container->Layers_List.size()].Image_Name=goodsModel->item(layer_id,1)->text();

        goodsModel->item(layer_id,2)->setText("图片");
        if(item->checkState()==Qt::Checked){
            //设置显示TODO
            Container->Images_List[layer_id-Container->Layers_List.size()].show();
        }else{
            //设置隐藏Flag TODO
            Container->Images_List[layer_id-Container->Layers_List.size()].hide();
        }
    }else{
        Container->Layers_List[layer_id].Layer_Name=goodsModel->item(layer_id,1)->text();
        if(Container->Layers_List[layer_id].Ob_Type==0){
            goodsModel->item(layer_id,2)->setText("点");
        }else if(Container->Layers_List[layer_id].Ob_Type==1){
            goodsModel->item(layer_id,2)->setText("线");
        }else if(Container->Layers_List[layer_id].Ob_Type==2){
            goodsModel->item(layer_id,2)->setText("面");
        }
        if(item->checkState()==Qt::Checked){
            for(int i=0 ;i<Container->Items_List.at(layer_id).Cur_Item.size();i++)
                Container->Items_List[layer_id].Cur_Item.at(i)->show();
        }else if(checked_bug!=0){
            checked_bug--;
        }
        else{
            for(int i =0;i<Container->Items_List.at(layer_id).Cur_Item.size();i++)
                Container->Items_List[layer_id].Cur_Item.at(i)->hide();
        }
    }
}
void MainWindow::slotCustomContextMenu(const QPoint &){
    QModelIndex index = ui->treeView->currentIndex();
    int layer_index = index.row();
    if(layer_index!=-1&&layer_index<Container->Layers_List.size()&&index.column()!=0){
        QMenu *menu = new QMenu;
        qDebug()<<index;
        menu->addAction(QString("属性"), this, SLOT(Show_Attr()));
        menu->addAction(QString("修改样式"), this, SLOT(Change_Style()));
        menu->addAction(QString("添加字段"), this, SLOT(Add_Attr_Name()));
        Change_Style_ID=layer_index;//设置当前修改的layer索引
        menu->exec(QCursor::pos());
        ui->treeView->setCurrentIndex(init_modeIndex);
    }
}
void MainWindow::Add_Attr_Name(){
    bool ok;
    if(Change_Style_ID>=Container->Layers_List.size())return;
    QString text = QInputDialog::getText(this, tr("新建图层属性字段"),
                                         tr("请输入字段名："), QLineEdit::Normal,
                                         "New Name", &ok);
    if (ok && !text.isEmpty()){
        Container->Add_Layer_Attr(Change_Style_ID,text);
    }
}
void MainWindow::attrItemChanged(QTableWidgetItem * item){
    int index = item->row();
    int attr_index = item->column();
    qDebug()<<index<<":"<<attr_index;
    int obtype=Container->Layers_List.at(index).Ob_Type;
    int jump=0;
    for(int i=0;i<Container->Layers_List.size();i++){
        if(Container->Layers_List.at(i)==obtype){
            jump+=Container->Layers_List.at(i).Size;
        }
    }
    if(obtype==0){
        Container->Points_List[jump+index].Attribute_Point[attr_index]=item->data(attr_index).toString();
    }else if(obtype==1){

        Container->Lines_List[jump+index].Attribute_Line[attr_index]=item->data(attr_index).toString();
    }else{

        Container->Polygens_List[jump+index].Attribute_Polygen[attr_index]=item->data(attr_index).toString();
    }


}

void MainWindow::Show_Attr(){
    int Attr_size=Container->Layers_List.at(Change_Style_ID).Attribute_Name.size();
    int Elements_size=Container->Layers_List.at(Change_Style_ID).Size;
    QTableWidget *tableWidget = new QTableWidget(Elements_size,Attr_size); // 构造了一个QTableWidget的对象
    tableWidget->setWindowTitle("查看属性");
    connect(tableWidget,
            SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(attrItemChanged(QTableWidgetItem *)));
    //     tableWidget->resize(350, 200);  //设置表格
    QStringList header;
    for(int i=0;i<Attr_size;i++){
        header<<Container->Layers_List.at(Change_Style_ID).Attribute_Name.at(i);
    }
    tableWidget->setHorizontalHeaderLabels(header);//设置表头
    int jump_index=0;
    for(int i=0;i<Change_Style_ID;i++){
        if(Container->Layers_List.at(i).Ob_Type==Container->Layers_List.at(Change_Style_ID).Ob_Type)
            jump_index+=Container->Layers_List.at(i).Size;
    }
    if(Container->Layers_List.at(Change_Style_ID).Ob_Type==0){
        for(int i=0;i<Elements_size;i++){
            for(int j=0;j<Container->Points_List.at(i+jump_index).Attribute_Point.size();j++){
                tableWidget->setItem(i,j,new QTableWidgetItem(Container->Points_List.at(i+jump_index).Attribute_Point.at(j)));
            }
        }
    }else if(Container->Layers_List.at(Change_Style_ID).Ob_Type==1){
        for(int i=0;i<Elements_size;i++){
            for(int j=0;j<Container->Lines_List.at(i+jump_index).Attribute_Line.size();j++)
                tableWidget->setItem(i,j,new QTableWidgetItem(Container->Lines_List.at(i+jump_index).Attribute_Line.at(j)));
        }
    }else{
        for(int i=0;i<Elements_size;i++){
            for(int j=0;j<Container->Polygens_List.at(i+jump_index).Attribute_Polygen.size();j++)
                tableWidget->setItem(i,j,new QTableWidgetItem(Container->Polygens_List.at(i+jump_index).Attribute_Polygen.at(j)));
        }
    }
    tableWidget->show();
}

void MainWindow::Change_Style(){
    ui->dockWidget->show();
}
void MainWindow::on_Save_Style_clicked()
{
    if(ui->penStyleComboBox->currentText() == QStringLiteral("实线"))
    {
        Container->Layers_List[Change_Style_ID].penStyle=Qt::SolidLine;
    }
    else if(ui->penStyleComboBox->currentText()  == QStringLiteral("虚线"))
    {
        Container->Layers_List[Change_Style_ID].penStyle=Qt::DotLine;
    }
    Container->Layers_List[Change_Style_ID].penWidth = ui->penWidthSpinBox->text().toInt();
    Container->Layers_List[Change_Style_ID].brushColor = Temp_Color_Brush;
    Container->Layers_List[Change_Style_ID].penColor = Temp_Color_Pen;

    //TODO 修改Container中的所有items
    QPen temp;
    temp.setWidth(Container->Layers_List[Change_Style_ID].penWidth);
    temp.setBrush(Temp_Color_Brush);
    temp.setColor(Temp_Color_Pen);
    int obtype=Container->Layers_List[Change_Style_ID].Ob_Type;
    for(int i =0;i< Container->Items_List.at(Change_Style_ID).Cur_Item.size();i++){
        if(obtype==0){
            QGraphicsEllipseItem *new_Point_style;
            new_Point_style=dynamic_cast<QGraphicsEllipseItem *>(Container->Items_List.at(Change_Style_ID).Cur_Item[i]);
            new_Point_style->setBrush(Temp_Color_Brush);
            new_Point_style->setPen(temp);
        }else if(obtype==1){

            QGraphicsPathItem* new_Line_style;
            new_Line_style=dynamic_cast<QGraphicsPathItem *>(Container->Items_List.at(Change_Style_ID).Cur_Item[i]);
            //new_Line_style->setBrush(Temp_Color_Brush);
            new_Line_style->setPen(temp);
        }else{
            QGraphicsPolygonItem* new_Polygen_style;
            new_Polygen_style=dynamic_cast<QGraphicsPolygonItem *>(Container->Items_List.at(Change_Style_ID).Cur_Item[i]);
            new_Polygen_style->setBrush(Temp_Color_Brush);
            new_Polygen_style->setPen(temp);
        }
    }

    Change_Style_ID=-1;

    ui->dockWidget->hide();
}

//////////////////////////编辑要素//////////////////////////
void MainWindow::on_action_Start_Edit_triggered()
{

    QStringList lst;
    for(int i=0;i<Container->Layers_List.size();i++){
        lst<<Container->Layers_List.at(i).Layer_Name;
    }
    if(Container->Layers_List.size()==0){
        lst<<" <空> ";
    }
    bool ok = FALSE;
    QString res = QInputDialog::getItem(this,
                                        tr( "选择图层" ),
                                        tr( "图层列表：" ), lst, 1, TRUE, &ok);
    if ( ok&&Container->Layers_List.size()!=0 )// 用户选择一项并且按下OK
    {
        Container->Layer_ID = Container->Layers_List.at(lst.indexOf(res)).Layer_ID;//修改当前编辑的图层号

        QColor brushColor=Qt::yellow;   //填充颜色
        QBrush brush(brushColor);
        checked_bug+=3;
        goodsModel->item(lst.indexOf(res))->setBackground(brush);
        goodsModel->item(lst.indexOf(res),1)->setBackground(brush);
        goodsModel->item(lst.indexOf(res),2)->setBackground(brush);
        ui->action_Draw->setEnabled(true);
        ui->action_Edit->setEnabled(true);
        //ui->action_Move->setEnabled(true);
        ui->action_Refresh->setEnabled(true);
        ui->action_Start_Edit->setEnabled(false);
        ui->menu_4->setEnabled(false);
        ui->action_End_Edit->setEnabled(true);
        //设置可被选中Flag TODO c
        for(int i=0;i<Container->Items_List.at(Container->Layer_ID).Cur_Item.size();i++){
            Container->Items_List[Container->Layer_ID].Cur_Item[i]->setFlag(QGraphicsItem::ItemIsSelectable,true);
        }
        //设置画笔
        int ob_type=Container->Layers_List.at(lst.indexOf(res)).Ob_Type;
        if(ob_type == 0)
        {
            area->setShape(EditWidget::PointType);
        }
        else if(ob_type == 1)
        {
            area->setShape(EditWidget::PolylineType);
        }
        else if(ob_type == 2)
        {
            area->setShape(EditWidget::PolygonType);
        }
    }
}

void MainWindow::on_action_End_Edit_triggered()
{

    QColor brushColor=Qt::white;   //填充颜色
    QBrush brush(brushColor);
    qDebug()<<goodsModel->rowCount();
    for(int i=0;i<goodsModel->rowCount();i++){
        checked_bug+=3;
        goodsModel->item(i)->setBackground(brush);
        goodsModel->item(i,1)->setBackground(brush);
        goodsModel->item(i,2)->setBackground(brush);
    }
    //设置不可选 TODO
    for(int i=0;i<Container->Items_List.at(Container->Layer_ID).Cur_Item.size();i++){
        Container->Items_List.at(Container->Layer_ID).Cur_Item.at(i)->setFlag(QGraphicsItem::ItemIsSelectable,false);
    }
    Container->Layer_ID=-1;
    ui->action_Draw->setEnabled(false);
    ui->action_Edit->setEnabled(false);
    //ui->action_Move->setEnabled(false);
    ui->action_Refresh->setEnabled(false);
    ui->action_Start_Edit->setEnabled(true);
    ui->menu_4->setEnabled(true);
    ui->action_End_Edit->setEnabled(false);
    //设置鼠标状态
    area->isDrawing=false;
    area->isEditing=false;
    area->setState(EditWidget::NoneState);
    area->views()[0]->setCursor(Qt::ArrowCursor);
}

////////////////创建点线面图层/////////////////////
void MainWindow::on_action_Create_PointLayer_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("新建点图层"),
                                         tr("请输入图层名："), QLineEdit::Normal,
                                         "New Point Layer", &ok);
    if (ok && !text.isEmpty()){
        Container->Add_Layer(text,0);
        Show_TreeView();
    }
}

void MainWindow::on_action_Create_LineLayer_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("新建线图层"),
                                         tr("请输入图层名："), QLineEdit::Normal,
                                         "New Line Layer", &ok);
    if (ok && !text.isEmpty()){
        Container->Add_Layer(text,1);
        Show_TreeView();
    }
}

void MainWindow::on_action_Create_PolygenLayer_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("新建面图层"),
                                         tr("请输入图层名："), QLineEdit::Normal,
                                         "New Polygen Layer", &ok);
    if (ok && !text.isEmpty()){
        Container->Add_Layer(text,2);
        Show_TreeView();
    }
}


void MainWindow::on_action_Show_LayerList_triggered()
{
    ui->dockWidget_Layers->show();
}
//数据库交互部分
void MainWindow::on_action_ReadDB_triggered()
{
    database.Connect();
    QStringList lst;
    int size=database.Get_Project_Size();
    qDebug()<<size;
    if(size==0){
        lst<<" <空> ";
    }else{
        lst=database.Get_Projcet();
    }
    bool ok = FALSE;
    QString res = QInputDialog::getItem(this,
                                        tr( "选择项目" ),
                                        tr( "项目列表：" ), lst, 1, TRUE, &ok);
    if (ok && size!=0&&Container->Project_ID==-1&&Container->Layers_List.size()==0){
        int index = lst.indexOf(res);
        QString temp =  lst.at(index);
        qDebug()<<temp.split(".").at(0).toInt();
        database.Get_Info(temp.split(".").at(0).toInt());
        Container->Project_ID=index+1;
        Show_TreeView();

    }else if(!ok||size==0){
        QMessageBox::information(this,"提示","数据库中无可选项目");
    }else if(Container->Project_ID!=-1||Container->Layers_List.size()!=0){
        QMessageBox::information(this,"提示","当前已经在编辑项目，无法重新读取");
    }

}

void MainWindow::on_action_Save_DataBase_triggered()
{
    database.Connect();
    if(Container->Project_ID==-1){
        bool ok;
        QString text = QInputDialog::getText(this, tr("新建矢量化项目"),
                                             tr("请输入项目名："), QLineEdit::Normal,
                                             "New Project", &ok);
        if (ok && !text.isEmpty()){
            database.Add_Project(text);
            database.Add_Info(database.Get_Project_Last());
            Container->Project_ID=database.Get_Project_Last()-1;
        }
    }else{
        database.Delete_Info(Container->Project_ID);
        database.Add_Info(Container->Project_ID);
    }
}
