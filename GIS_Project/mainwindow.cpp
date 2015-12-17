#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editwidget.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QColorDialog>
#include <QMessageBox>
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
    ui->pushSent->setEnabled(false);
    this->ui->timeBut->setEnabled(false);
    //开始读取
    tcpClient->abort();
    connect(tcpClient,&QTcpSocket::readyRead,
            [&](){
        qDebug()<<tr("Server Say：%1").arg(QString(this->tcpClient->readAll()));
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
    h_time=1;
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

}

void MainWindow::on_action_ZoomIn_triggered()
{
    area->views()[0]->scale(1.2,1.2);
}

void MainWindow::on_action_ZoomOut_triggered()
{
    area->views()[0]->scale(1/1.2,1/1.2);
}


void MainWindow::on_drawpoints_clicked()
{
    area->showpoints();
}
//tcp部分------------------------------------------------------------

void MainWindow::on_pushConnect_clicked()//连接服务器
{
    qDebug() << "点击连接：" ;
    if ("连接" == this->ui->pushConnect->text())
    {
        tcpClient->connectToHost(ipAdd,portd.toInt());
        if (tcpClient->waitForConnected(1000))
        {
            ui->pushConnect->setText("断开");
            qDebug()<<"连接服务器成功";
            ui->pushSent->setEnabled(true);
            this->ui->timeBut->setEnabled(true);
        }
    }
    else
    {
        tcpClient->disconnectFromHost();
        if (tcpClient->state() == QAbstractSocket::UnconnectedState || tcpClient->waitForDisconnected(1000) )
        {
            ui->pushConnect->setText("连接");
            qDebug()<<"断开服务器";
            ui->pushSent->setEnabled(false);
            tm.stop();
            this->ui->timeBut->setEnabled(false);
            this->ui->timeBut->setText("启动定时");
        }
    }
}

void MainWindow::on_pushSent_clicked()//发送信息
{
    qDebug() << "点击发送：" ;
    QString data =" this->ui->txtData->text();";
    if (data.isEmpty())
    {
        return ;
    }
    tcpClient->write(data.toUtf8());
}

void MainWindow::ReadError(QAbstractSocket::SocketError)//出错
{
    tcpClient->disconnectFromHost();
    ui->pushConnect->setText("连接");
    qDebug()<<tr("连接出错：%1").arg(tcpClient->errorString());
    ui->pushSent->setEnabled(false);
    tm.stop();
    this->ui->timeBut->setEnabled(false);
    this->ui->timeBut->setText("启动定时");
}

void MainWindow::on_timeBut_clicked()//设置定时
{
    if ("启动定时" == this->ui->timeBut->text())
    {
        int h;
        h = h_time*1000;
        tm.start(h);
        this->ui->timeBut->setText("停止定时");
    }
    else
    {
        tm.stop();
        this->ui->timeBut->setText("启动定时");
    }
}


