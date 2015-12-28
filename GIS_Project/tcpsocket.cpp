#include "tcpsocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>
#include <struct_list.h>
#include <mydatastream.h>
TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
    connect(this,&TcpSocket::readyRead,this,&TcpSocket::readData);
    dis = connect(this,&TcpSocket::disconnected,
                  [&](){
        qDebug() << "disconnect" ;
        emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
        this->deleteLater();
    });
    connect(&watcher,&QFutureWatcher<QByteArray>::finished,this,&TcpSocket::startNext);
    connect(&watcher,&QFutureWatcher<QByteArray>::canceled,this,&TcpSocket::startNext);
    qDebug() << "new connect" ;
}

TcpSocket::~TcpSocket()
{
}


void TcpSocket::sentData(const QByteArray &data, const int id)
{
    qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!!";
    if(id == socketID)
    {
        write(data);
    }
}

void TcpSocket::disConTcp(int i)
{
    if (i == socketID)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        this->deleteLater();
    }
}
QByteArray TcpSocket::intToByte(int i)
{
    QByteArray abyte0;
    abyte0.resize(4);
    abyte0[0] = (uchar)  (0x000000ff & i);
    abyte0[1] = (uchar) ((0x0000ff00 & i) >> 8);
    abyte0[2] = (uchar) ((0x00ff0000 & i) >> 16);
    abyte0[3] = (uchar) ((0xff000000 & i) >> 24);
    return abyte0;
}
int TcpSocket::bytesToInt(QByteArray bytes) {
    int addr = bytes[0] & 0x000000FF;
    addr |= ((bytes[1] << 8) & 0x0000FF00);
    addr |= ((bytes[2] << 16) & 0x00FF0000);
    addr |= ((bytes[3] << 24) & 0xFF000000);
    return addr;
}
void TcpSocket::readData()
{
    qDebug()<<"point:"<<Container->Points_List.size();
    qDebug()<<"items:"<<Container->Items_List.at(0).Cur_Item.size();
    mutex.lock();
    QByteArray block;//保存要发送的二进制数据
    QByteArray Byte_size,Byte_ID;
    QDataStream Message(this->readAll());//读取所有发来的信息
    Message>>Byte_ID;
    int PC_ID_New = bytesToInt(Byte_ID);//取出连接pc的ID号

    Message>>Byte_size;
    int size = 0;
    size = bytesToInt(Byte_size);//取出图层数量
    if(size==0){//假如为空，表示为一个新的PC加入
        for(int i = 0;i<Container->Layers_List.size();i++){//添加Pc_numbers
            Container->Layers_List[i].Pc_numbers++;
        }
        QDataStream out (&block,QIODevice::WriteOnly);//输出流
        int layer_size=Container->Layers_List.size();
        out<<intToByte(layer_size);
        for(int i=0;i<layer_size;i++){
            out<<Container->Layers_List.at(i);
        }
        int Point_size=Container->Points_List.size();
        out<<intToByte(Point_size);
        for(int i=0;i<Point_size;i++){
            out<<Container->Points_List.at(i);
        }
        int Line_size=Container->Lines_List.size();
        out<<intToByte(Line_size);
        for(int i=0;i<Line_size;i++){
            out<<Container->Lines_List.at(i);
        }
        int Polygen_size=Container->Polygens_List.size();
        out<<intToByte(Polygen_size);
        for(int i=0;i<Polygen_size;i++){
            out<<Container->Polygens_List.at(i);
        }
        this->write(block);
    }else{

        //Step1:将本地的操作修改进行分析后打包
        //先把本地容器中的图层全部打包
        QDataStream out (&block,QIODevice::WriteOnly);
        //        int PC_id_sent=Container->PC_ID;
        //        out<<intToByte(PC_id_sent);
        int layer_size=Container->Layers_List.size();
        out<<intToByte(layer_size);
        int Point_Mod_size=0,Line_Mod_size=0,Polygen_Mod_size=0;//记录修改操作中的3种要素的数量

        for(int i=0;i<layer_size;i++){
            qDebug()<<"accept pc1 size"<<Container->Layers_List.at(i).Accept_PC.at(0).size();
            //统计各修改元素的数量
            if(Container->Layers_List.at(i).Ob_Type==0){
                for(int j =0;j<Container->Layers_List.at(i).PC_ID.size();j++){

                        for(int k=0;k<Container->Layers_List[i].Accept_PC.at(j).size();k++){
                            if(Container->Layers_List[i].Accept_PC.at(j).toInt()==Container->PC_ID)
                                Container->Layers_List[i].Accept_PC[j]="";
                        }
                        qDebug()<<"accept pc size"<<Container->Layers_List.at(i).Accept_PC.at(0).size();
                        if(Container->Layers_List[i].Accept_PC.at(j).size()>=Container->Layers_List.at(i).Pc_numbers){//冗余值发现
                            qDebug()<<"删除冗余";
                            Container->Layers_List[i].PC_ID.removeAt(j);
                            Container->Layers_List[i].Index_Part.removeAt(j);
                            Container->Layers_List[i].Change_Way.removeAt(j);
                            Container->Layers_List[i].Accept_PC.removeAt(j);
                        }else{

                            Point_Mod_size++;
                        }
                }
            }else if(Container->Layers_List.at(i).Ob_Type==1){

                for(int j =0;j<Container->Layers_List.at(i).PC_ID.size();j++){

                        if(Container->Layers_List[i].Accept_PC.at(j).size()>=Container->Layers_List.at(i).Pc_numbers){//冗余值发现
                            qDebug()<<"删除冗余";
                            Container->Layers_List[i].PC_ID.removeAt(j);
                            Container->Layers_List[i].Index_Part.removeAt(j);
                            Container->Layers_List[i].Change_Way.removeAt(j);
                            Container->Layers_List[i].Accept_PC.removeAt(j);
                        }else{

                            Line_Mod_size++;
                        }
                }
            }else{
                for(int j =0;j<Container->Layers_List.at(i).PC_ID.size();j++){

                        if(Container->Layers_List[i].Accept_PC.at(j).size()>=Container->Layers_List.at(i).Pc_numbers){//冗余值发现
                            qDebug()<<"删除冗余";
                            Container->Layers_List[i].PC_ID.removeAt(j);
                            Container->Layers_List[i].Index_Part.removeAt(j);
                            Container->Layers_List[i].Change_Way.removeAt(j);
                            Container->Layers_List[i].Accept_PC.removeAt(j);
                        }else{

                            Polygen_Mod_size++;
                        }
                }
            }
            out<<Container->Layers_List.at(i);
        }
        //从图层中查找到点图层，收集修改信息，将修改的要素从对应点容器中提取出来放入输出流中
        out<<intToByte(Point_Mod_size);
        for(int i=0;i<layer_size;i++){
            if(Container->Layers_List.at(i).Ob_Type==0){
                for(int j=0;j<Container->Layers_List.at(i).PC_ID.size();j++){
                    int pc_id = Container->Layers_List.at(i).PC_ID.at(j);//获取操作记录中对应的三层记录值
                    int index = Container->Layers_List.at(i).Index_Part.at(j);

                    Container->Layers_List[i].Accept_PC[j].append(QString::number(pc_id));
                    out<<Container->Points_List.at(Container->Current_search(Container->Layers_List.at(i).Layer_ID,pc_id,index,0));
                    //识别Accpet中是否有对应PCid,分发完毕则执行冗余清除工作
                    qDebug()<<"accept PCs"<<Container->Layers_List[i].Accept_PC[j];
                    qDebug()<<Container->Layers_List[i].Accept_PC.at(j).size();

                }
            }
        }
        out<<intToByte(Line_Mod_size);
        for(int i=0;i<layer_size;i++){
            if(Container->Layers_List.at(i).Ob_Type==1){
                for(int j=0;j<Container->Layers_List.at(i).PC_ID.size();j++){
                    int pc_id = Container->Layers_List.at(i).PC_ID.at(j);
                    int index = Container->Layers_List.at(i).Index_Part.at(j);

                    Container->Layers_List[i].Accept_PC[j].append(QString::number(pc_id));
                    out<<Container->Lines_List.at(Container->Current_search(Container->Layers_List.at(i).Layer_ID,pc_id,index,1));
                    //识别Accpet中是否有对应PCid,分发完毕则执行冗余清除工作
                }
            }
        }
        out<<intToByte(Polygen_Mod_size);
        for(int i=0;i<layer_size;i++){
            if(Container->Layers_List.at(i).Ob_Type==2){
                for(int j=0;j<Container->Layers_List.at(i).PC_ID.size();j++){
                    int pc_id = Container->Layers_List.at(i).PC_ID.at(j);
                    int index = Container->Layers_List.at(i).Index_Part.at(j);

                    Container->Layers_List[i].Accept_PC[j].append(QString::number(pc_id));
                    out<<Container->Polygens_List.at(Container->Current_search(Container->Layers_List.at(i).Layer_ID,pc_id,index,2));
                    //识别Accpet中是否有对应PCid,分发完毕则执行冗余清除工作
                }
            }
        }
        this->write(block);
        //Step2:将客户端的修改内容添加到服务器的容器中，并对相应图元进行删除操作
        for(int i=0;i<size;i++){
            St_Layers Layers_out;
            Message>>Layers_out;
            qDebug()<<"layers_out::"<<Layers_out.PC_ID;
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
                    if(Layers_out.Change_Way.at(t)==0){
                        Container->Layers_List[Container->Search_Layer_Index(Layers_out.Layer_ID)].Every_size[Layers_out.PC_ID.at(t)]--;
                        Container->Layers_List[Container->Search_Layer_Index(Layers_out.Layer_ID)].Size--;
                    }
                }
                //把客户机上修改的内容保存到服务器的容器中

                Container->Layers_List[Container->Search_Layer_Index(Layers_out.Layer_ID)].PC_ID.append(Layers_out.PC_ID.at(t));
                Container->Layers_List[Container->Search_Layer_Index(Layers_out.Layer_ID)].Index_Part.append(Layers_out.Index_Part.at(t));
                Container->Layers_List[Container->Search_Layer_Index(Layers_out.Layer_ID)].Change_Way.append(Layers_out.Change_Way.at(t));
                Container->Layers_List[Container->Search_Layer_Index(Layers_out.Layer_ID)].Accept_PC.append(QString::number(PC_ID_New, 10));
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


            Container->Layers_List[Points_out.Layer_ID].Every_size[Points_out.PC_ID]++;
            Container->Layers_List[Points_out.Layer_ID].Size++;
            St_Temp temp_receive;
            temp_receive.Layer_ID=Points_out.Layer_ID;
            temp_receive.PC_ID=Points_out.PC_ID;
            temp_receive.Index_Part=Points_out.Index_Part;
            temp_receive.Type=0;
            Container->Tcp_Receive.append(temp_receive);
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



            Container->Layers_List[Lines_out.Layer_ID].Every_size[Lines_out.PC_ID]++;
            Container->Layers_List[Lines_out.Layer_ID].Size++;
            St_Temp temp_receive;
            temp_receive.Layer_ID=Lines_out.Layer_ID;
            temp_receive.PC_ID=Lines_out.PC_ID;
            temp_receive.Index_Part=Lines_out.Index_Part;
            temp_receive.Type=1;
            Container->Tcp_Receive.append(temp_receive);
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


            Container->Layers_List[Polygens_out.Layer_ID].Every_size[Polygens_out.PC_ID]++;
            Container->Layers_List[Polygens_out.Layer_ID].Size++;
            St_Temp temp_receive;
            temp_receive.Layer_ID=Polygens_out.Layer_ID;
            temp_receive.PC_ID=Polygens_out.PC_ID;
            temp_receive.Index_Part=Polygens_out.Index_Part;
            temp_receive.Type=2;
            Container->Tcp_Receive.append(temp_receive);
        }
    }
    block.clear();//倒掉
    //    auto data  = handleData(this->readAll(),this->peerAddress().toString(),this->peerPort());
    //    auto test =this->readAll();
    //    qDebug() << data;
    //    this->write(test);
    //    this->write(data);
    //    if (!watcher.isRunning())//放到异步线程中处理。
    //    {
    //        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue()));
    //    }
    mutex.unlock();
}
QByteArray TcpSocket::handleData(QByteArray data)
{
    return data;
}

void TcpSocket::startNext()
{
    this->write(watcher.future().result());
    if (!datas.isEmpty())
    {
        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue()));
    }
}
void TcpSocket::Get_Container(Container_List &Container_out){
    Container=&Container_out;
}
