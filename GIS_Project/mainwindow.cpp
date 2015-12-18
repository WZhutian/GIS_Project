#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editwidget.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QColorDialog>
#include <QMessageBox>
#include "mydatastream.h"
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
//折半查找对应的要素,参数为：3层标记，图元类型
//int MainWindow::Current_search(int Layer_ID,int PC_ID,int Index_Part,int Type)
//{
//    int len;
//    if(Type==0){
//        len=Container->Points_List.size();
//    }else if()
// int high=len-1,low=0,mid;
// mid=(high+low)/2;
// while(high>=low)
// {
//  if(target>data[mid])
//   low=mid+1;
//  else if(target<data[mid])
//   high=mid-1;
//  else return mid;
//  mid=(high+low)/2;
// }
// return -1;
//}
//int MainWindow::Add_search(int Layer_ID,int PC_ID,int Type)
//{
//    int len;
//    if(Type==0){
//        len=Container->Points_List.size();
//    }else if()
// int high=len-1,low=0,mid;
// mid=(high+low)/2;
// while(high>=low)
// {
//  if(target>data[mid])
//   low=mid+1;
//  else if(target<data[mid])
//   high=mid-1;
//  else return mid;
//  mid=(high+low)/2;
// }
// return -1;
//}
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
        Container_List Container_out;
        for(int i=0;i<size;i++){
            St_Layers Layers_out;
            Message>>Layers_out;
            Container_out.Layers_List.append(Layers_out);
             //对发来的layers做分析，先分析layer中操作，对容器进行修改，存在的都删除；
            for(int t=0;t<Layers_out.PC_ID.size();t++){
//                if(Layers_out.Change_Way==2){//添加

//                }else{
//                    int judge_none=1;//假设本地存在layer
//                    int index;
//                    //先找到对应的Layer
//                    for(int f=0;f<Container->Layers_List.size();f++){
//                        if(Container->Layers_List.at(f).Layer_ID==Layers_out.Layer_ID){
//                            int index_before=0;
//                            //记录在此之前的总大小
//                            for(int c=0;c<Container->Layers_List.at(f).PC_ID;c++)
//                                index_before+=Container->Layers_List.at(f).Every_size[c];
//                            //找到
//                            for(c=0;c<Container->Layers_List.at(f).Every_size[Container->Layers_List.at(f).PC_ID];c++)

//                            //删除操作

//                            break;
//                        }else{//没有这个图层，说明全部都为新的，不需要操作
//                           judge_none=0;
//                        }
//                    }
//                    if(judge_none==0)break;
//                }
            }
        }

        //按顺序添加后面的包
        QByteArray Pt_size;
        Message>>Pt_size;
        int size_Pt=bytesToInt(Pt_size);
        for(int j=0;j<size_Pt;j++){
            St_Points Points_out;
            Message>>Points_out;
            Container_out.Points_List.append(Points_out);
        }
//        QByteArray Ln_size;
//        Message>>Ln_size;
//        int size_Ln=bytesToInt(Ln_size);
//        for(j=0;j<size_Ln;j++){
//            St_Lines Lines_out;
//            Message>>Lines_out;
//            Container_out.Lines_List.append(Lines_out);
//        }
//        QByteArray Pl_size;
//        Message>>Pl_size;
//        int size_Pl=bytesToInt(Pl_size);
//        for(j=0;j<size_Pl;j++){
//            St_Polygens Polygens_out;
//            Message>>Polygens_out;
//            Container_out.Polygens_List.append(Polygens_out);
//        }
//        //把本地的layer替换为服务器传递过来的layer,并清空通信判断部分
//        for(i=0;i<size;i++){
//            Container_out.Layers_List.at(i).PC_ID.clear();
//            Container_out.Layers_List.at(i).Index_Part.clear();
//            Container_out.Layers_List.at(i).Change_Way.clear();
//            Container_out.Layers_List.at(i).Accept_PC.clear();
//            Container.Layers_List.at(i)=Container_out.Layers_List.at(i);
//        }
    });
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ReadError(QAbstractSocket::SocketError)));//错误信号
    connect(&tm,&QTimer::timeout,[&](){
        int i = qrand() % 6;
        qDebug()<<tr("%1 Timer Sent: %2").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(list.at(i));
        tcpClient->write(list.at(i).toUtf8());
    });//循环
    connect(tcpClient,&QTcpSocket::disconnected,[](){qDebug()<< "123333" ;});
    list << "我是谁?" << "渡世白玉" << "hello" << "哈哈哈哈哈" << "你是坏蛋!" <<  "测试一下下了" << "不知道写什么" ;
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    ipAdd="127.0.0.1";
    portd="6666";
    h_time=10;
}

void MainWindow::Get_Container(Container_List &Container_Out){
    Container=&Container_Out;
    area->Get_Container(*Container);
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
    }
}



void MainWindow::on_action_Refresh_triggered()
{
    area->clear();
    qDebug()<<area->items().size();
    for(int i=0;i<area->containerItem.count();i++)
    {
        area->addItem(area->containerItem[i]);
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
        tcpClient->connectToHost(ipAdd,portd.toInt());
        if (tcpClient->waitForConnected(1000))
        {
            ui->action_Tcp_Connect->setText("断开");
            qDebug()<<"连接服务器成功";
            ui->action_Tcp_Sent->setEnabled(true);
            this->ui->action_Tcp_Time->setEnabled(true);
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
    }
}

void MainWindow::on_action_Tcp_Sent_triggered()
{
    qDebug() << "点击发送：" ;
    QString data =" this->ui->txtData->text();";
    if (data.isEmpty())
    {
        return ;
    }
    tcpClient->write(data.toUtf8());
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

void MainWindow::on_action_Edit_2_triggered()
{

}
