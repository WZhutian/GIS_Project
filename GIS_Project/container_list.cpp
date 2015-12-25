#include "container_list.h"
#include<QColor>
#include <QDebug>
Container_List::Container_List()
{
    Layer_ID=-1;//初始化为-1，表示无正在编辑图层
}
//折半查找对应的要素,参数为：3层标记，图元类型
////////////////////////增删改//////////////////////////////////
void Container_List::Add_Point_Item(QPointF Point_Item_Out,int index)
{
    QGraphicsEllipseItem *newPointCircle=
            new QGraphicsEllipseItem(Point_Item_Out.x()-5,Point_Item_Out.y()-5,10,10);
   // newPointCircle->setBrush(Qt::red);
    newPointCircle->setFlag(QGraphicsItem::ItemIsMovable,true);
    newPointCircle->setFlag(QGraphicsItem::ItemIsSelectable,true);
    newPointCircle->setData(0,PC_ID);
    newPointCircle->setData(1,Layer_ID);
    newPointCircle->setData(2,index);
    Items_List[Layer_ID].Cur_Item.append(newPointCircle);
}


void Container_List::Add_Point(QPointF Point_Out){
    qDebug()<<Layers_List.at(0).Every_size[0];
    int index =  Add_search(Layer_ID,PC_ID,0);
    int Part_Index=0;
    if(index>0){
        if(Points_List.at(index-1).Layer_ID==Layer_ID&&
                Points_List.at(index-1).PC_ID==PC_ID){
            if(Points_List.at(index-1).Index_Part!=0){
                Part_Index=Points_List.at(index-1).Index_Part+1;
            }else
                Part_Index=0;
        }
    }else{
        Part_Index=0;
    }
    //添加Items
    this->Add_Point_Item(Point_Out,Part_Index);
    //添加数据部分
    St_Points Addin;
    Addin.Point=Point_Out;
    Addin.Layer_ID=Layer_ID;
    Addin.PC_ID=PC_ID;
    Addin.Index_Part=Part_Index;
    Points_List.insert(index,Addin);//TODO
    //添加图层部分
    int layer_index = Search_Layer_Index(Layer_ID);
    Layers_List[layer_index].Every_size[PC_ID]++;
    Layers_List[layer_index].Size++;
    Layers_List[layer_index].PC_ID.append(PC_ID);
    Layers_List[layer_index].Index_Part.append(index);
    Layers_List[layer_index].Change_Way.append(2);
}
void Container_List::Modify_Point_Item(QPointF Point_Item_Out_New,int index)
{
    QGraphicsItem *origPointCricle=Items_List[Layer_ID].Cur_Item.at(index);
    origPointCricle->setPos(Point_Item_Out_New.x(),Point_Item_Out_New.y());
}

void Container_List::Modify_Point(int Index_Part_Out,QPointF Point_Out_New){
    int index =  Current_search(Layer_ID,PC_ID,Index_Part_Out,0);
    //修改Items部分
    this->Modify_Point_Item(Point_Out_New,index);
    //修改数据部分
    Points_List[index].Point=Point_Out_New;
    //添加图层部分
    int layer_index = Search_Layer_Index(Layer_ID);
    //搜索全部修改部分，查找是否有相同记录,从前往后开始查找
    for(int i=0;Layers_List.at(layer_index).PC_ID.size();i--){
        if(Layers_List.at(layer_index).Index_Part.at(i)==index){//已经有的就不修改
            return;
        }else if(Layers_List.at(layer_index).Change_Way.at(i)==2){
            break;
        }
    }
    //解决查询效率问题：将每次修改的内容添加到数组的开头，
    Layers_List[layer_index].PC_ID.insert(0,PC_ID);
    Layers_List[layer_index].Index_Part.insert(0,index);
    Layers_List[layer_index].Change_Way.insert(0,1);
}
void Container_List::Delete_Point_Item(int Index)
{
    QGraphicsItem *curClearItem=Items_List[Layer_ID].Cur_Item[Index];
    Items_List[Layer_ID].Cur_Item.removeOne(curClearItem);
}

void Container_List::Delete_Point(int Index_Part_Out){
    int index=Current_search(Layer_ID,PC_ID,Index_Part_Out,0);
    //修改Items部分
    this->Delete_Point_Item(index);
    //修改数据部分
    Points_List.removeAt(Index_Part_Out);
    //添加图层部分
    int layer_index = Search_Layer_Index(Layer_ID);
    Layers_List[layer_index].Every_size[PC_ID]--;
    Layers_List[layer_index].Size--;
    Layers_List[layer_index].PC_ID.insert(0,PC_ID);
    Layers_List[layer_index].Index_Part.insert(0,index);
    Layers_List[layer_index].Change_Way.insert(0,0);
}
/**
 * @brief Add_Line
 * @param Line_Out
 */
void Container_List::Add_Line_Item(QVector<QPointF> Line_Item_Out,int index)
{
    QPainterPath *path=new QPainterPath(Line_Item_Out[0]);
    for(int i=1;i<Line_Item_Out.count();++i)
    {
        path->lineTo(Line_Item_Out[i]);
    }
    QGraphicsPathItem *cur=new QGraphicsPathItem();
    cur->setPath(*path);
    //cur->setPen(*pen);
    cur->setFlag(QGraphicsItem::ItemIsMovable,true);
    cur->setFlag(QGraphicsItem::ItemIsSelectable,true);
    cur->setData(0,PC_ID);
    cur->setData(1,Layer_ID);
    cur->setData(2,index);
   Items_List[Layer_ID].Cur_Item.append(cur);

}

void Container_List::Add_Line(QVector<QPointF> Line_Out){
    int index =  Add_search(Layer_ID,PC_ID,1);
    int Part_Index=0;
    if(index!=0){
        if(Lines_List.at(index-1).Layer_ID==Layer_ID&&
                Lines_List.at(index-1).PC_ID==PC_ID){
            if(Lines_List.at(index-1).Index_Part!=0){
                Part_Index=Lines_List.at(index-1).Index_Part+1;
            }
        }
    }
    //添加Items
    this->Add_Line_Item(Line_Out,Part_Index);
    //添加数据部分
    St_Lines Addin;
    for(int i=0;i<Line_Out.size();i++){
        Addin.Line_FromTo.append(Line_Out.at(i));
    }
    Addin.Layer_ID=Layer_ID;
    Addin.PC_ID=PC_ID;
    Addin.Index_Part=Part_Index;
    Lines_List.insert(index,Addin);
    //添加图层部分
    int layer_index = Search_Layer_Index(Layer_ID);
    Layers_List[layer_index].Every_size[PC_ID]++;
    Layers_List[layer_index].Size++;
    Layers_List[layer_index].PC_ID.append(PC_ID);
    Layers_List[layer_index].Index_Part.append(index);
    Layers_List[layer_index].Change_Way.append(2);
}
void Container_List::Modify_Line_Item(QPointF Line_Item_Out_new, int index,int index_Line)
{
    QGraphicsItem *curEditItem=Items_List[Layer_ID].Cur_Item.at(index);
    QGraphicsPathItem *curEditPathItem = dynamic_cast<QGraphicsPathItem*>(curEditItem);
    Lines_List[index].Line_FromTo[index_Line]=Line_Item_Out_new;
    if(curEditPathItem!=NULL)
    {
        QPainterPath *path=new QPainterPath(Lines_List[index].Line_FromTo[0]);
        for(int i=1;i<Lines_List[index].Line_FromTo.count();++i)
        {
            path->lineTo(Lines_List[index].Line_FromTo[i]);
        }
        curEditPathItem->setPath(*path);
    }
}

void Container_List::Modify_Line(int Index_Part_Out,int Index_Line,QPointF Line_Out_New){
    int index =  Current_search(Layer_ID,PC_ID,Index_Part_Out,0);
    //修改Items部分
    this->Modify_Line_Item(Line_Out_New,index,Index_Line);
    //修改数据部分
    Lines_List[index].Line_FromTo[Index_Line]=Line_Out_New;
    //添加图层部分
    int layer_index = Search_Layer_Index(Layer_ID);
    //搜索全部修改部分，查找是否有相同记录,从前往后开始查找
    for(int i=0;Layers_List.at(layer_index).PC_ID.size();i--){
        if(Layers_List.at(layer_index).Index_Part.at(i)==index){//已经有的就不修改
            return;
        }else if(Layers_List.at(layer_index).Change_Way.at(i)==2){
            break;
        }
    }
    //解决查询效率问题：将每次修改的内容添加到数组的开头，
    Layers_List[layer_index].PC_ID.insert(0,PC_ID);
    Layers_List[layer_index].Index_Part.insert(0,index);
    Layers_List[layer_index].Change_Way.insert(0,1);
}
void Container_List::Delete_Line_Item(int Index)
{
    QGraphicsItem *curClearItem=Items_List[Layer_ID].Cur_Item[Index];
    Items_List[Layer_ID].Cur_Item.removeOne(curClearItem);
}

void Container_List::Delete_Line(int Index_Part_Out){
    int index=Current_search(Layer_ID,PC_ID,Index_Part_Out,0);
    //修改Items部分
    this->Delete_Line_Item(index);
    //修改数据部分
    Lines_List.removeAt(Index_Part_Out);
    //添加图层部分
    int layer_index = Search_Layer_Index(Layer_ID);
    Layers_List[layer_index].Every_size[PC_ID]--;
    Layers_List[layer_index].Size--;
    Layers_List[layer_index].PC_ID.insert(0,PC_ID);
    Layers_List[layer_index].Index_Part.insert(0,index);
    Layers_List[layer_index].Change_Way.insert(0,0);
}
/**
 * @brief Add_Polygen
 * @param Index_Part
 * @param Index_Polygen
 * @param Line_Out_New
 */
void Container_List::Add_Polygen_Item(QVector<QPointF> Poly_Item_Out,int index)
{
    QGraphicsPolygonItem *cur=new QGraphicsPolygonItem();
    QPolygonF *curPolygon=new QPolygonF(Poly_Item_Out);
    cur->setPolygon(*curPolygon);
    cur->setFlag(QGraphicsItem::ItemIsMovable,true);
    cur->setFlag(QGraphicsItem::ItemIsSelectable,true);
//    cur->setPen(*pen);
//    cur->setBrush(brushColor);
    cur->setData(0,PC_ID);
    cur->setData(1,Layer_ID);
    cur->setData(2,index);
    Items_List[Layer_ID].Cur_Item.append(cur);
}

void Container_List::Add_Polygen(QVector<QPointF> Polygen_Out){
    int index =  Add_search(Layer_ID,PC_ID,1);
    int Part_Index=0;
    if(index!=0){
        if(Polygens_List.at(index-1).Layer_ID==Layer_ID&&
                Polygens_List.at(index-1).PC_ID==PC_ID){
            if(Polygens_List.at(index-1).Index_Part!=0){
                Part_Index=Polygens_List.at(index-1).Index_Part+1;
            }
        }
    }
    //添加Items
    this->Add_Polygen_Item(Polygen_Out,Part_Index);
    //添加数据部分
    St_Polygens Addin;
    for(int i=0;i<Polygen_Out.size();i++){
        Addin.Polygen_Round.append(Polygen_Out.at(i));
    }
    Addin.Layer_ID=Layer_ID;
    Addin.PC_ID=PC_ID;
    Addin.Index_Part=Part_Index;
    Polygens_List.insert(index,Addin);
    //添加图层部分
    int layer_index = Search_Layer_Index(Layer_ID);
    Layers_List[layer_index].Every_size[PC_ID]++;
    Layers_List[layer_index].Size++;
    Layers_List[layer_index].PC_ID.append(PC_ID);
    Layers_List[layer_index].Index_Part.append(index);
    Layers_List[layer_index].Change_Way.append(2);
}
void Container_List::Modify_Polygen_Item(QPointF Polygens_Item_Out_new, int index, int Index_Polygen)
{
    QGraphicsItem *curEditItem =Items_List[Layer_ID].Cur_Item.at(index);
    QGraphicsPolygonItem *curEditPolygenItem =dynamic_cast<QGraphicsPolygonItem*>(curEditItem);
    Polygens_List[index].Polygen_Round[Index_Polygen]=Polygens_Item_Out_new;
    if(curEditPolygenItem!=NULL){
    QPolygonF *newPolygen =new QPolygonF(Polygens_List[index].Polygen_Round);
    curEditPolygenItem->setPolygon(*newPolygen);
    }
}

void Container_List::Modify_Polygen(int Index_Part_Out,int Index_Polygen,QPointF Polygen_Out_New){
    int index =  Current_search(Layer_ID,PC_ID,Index_Part_Out,0);
    //修改Items 部分
    this->Modify_Polygen_Item(Polygen_Out_New,index,Index_Polygen);
    //修改数据部分
    Polygens_List[index].Polygen_Round[Index_Polygen]=Polygen_Out_New;
    //添加图层部分
    int layer_index = Search_Layer_Index(Layer_ID);
    //搜索全部修改部分，查找是否有相同记录,从前往后开始查找
    for(int i=0;Layers_List.at(layer_index).PC_ID.size();i--){
        if(Layers_List.at(layer_index).Index_Part.at(i)==index){//已经有的就不修改
            return;
        }else if(Layers_List.at(layer_index).Change_Way.at(i)==2){
            break;
        }
    }
    //解决查询效率问题：将每次修改的内容添加到数组的开头，
    Layers_List[layer_index].PC_ID.insert(0,PC_ID);
    Layers_List[layer_index].Index_Part.insert(0,index);
    Layers_List[layer_index].Change_Way.insert(0,1);
}
void Container_List::Delete_Polygen_Item(int Index)
{
    QGraphicsItem *curClearItem=Items_List[Layer_ID].Cur_Item[Index];
    Items_List[Layer_ID].Cur_Item.removeOne(curClearItem);
}

void Container_List::Delete_Polygen(int Index_Part_Out){
    int index=Current_search(Layer_ID,PC_ID,Index_Part_Out,0);
    //修改Items部分
    this->Delete_Polygen_Item(index);
    //修改数据部分
    Polygens_List.removeAt(Index_Part_Out);
    //添加图层部分
    int layer_index = Search_Layer_Index(Layer_ID);
    Layers_List[layer_index].Every_size[PC_ID]--;
    Layers_List[layer_index].Size--;
    Layers_List[layer_index].PC_ID.insert(0,PC_ID);
    Layers_List[layer_index].Index_Part.insert(0,index);
    Layers_List[layer_index].Change_Way.insert(0,0);
}
//
void Container_List::Add_Layer(QString Layer_Name,int Ob_Type){
    St_Layers temp_ly;
    if(Layers_List.size()!=0){
         temp_ly.Layer_ID=Layers_List.at(Layers_List.size()-1).Layer_ID+1;
    }else{
        temp_ly.Layer_ID=0;
    }
    temp_ly.Layer_Name=Layer_Name;
    temp_ly.Ob_Type=Ob_Type;

    Layers_List.append(temp_ly);

    St_Items temp_items;
    Items_List.append(temp_items);

}
void Container_List::Add_Layer_Attr(int Layer_ID_Out,QString Attribute_Name){
    Layers_List[Search_Layer_Index(Layer_ID_Out)].Attribute_Name.append(Attribute_Name);
}
/////////////////////////查找///////////////////
int Container_List::Search_Layer_Index(int Layer_ID_Out){
    for(int j=0;j<Layers_List.size();j++){//先找到layer
        if(Layers_List.at(j).Layer_ID==Layer_ID_Out){
            return j;
        }
    }
    return -1;
}
int Container_List::Current_search(int Layer_ID,int PC_ID,int Index_Part,int Type)//TODO最后一步替换为二分查找
{
    int Index_Return=-1;//返回为-1则表示未找到
    if(Type==0){
        int len=Points_List.size();
        bool finished=false;//判断是否停止
        //先找到对应的Layer
        for(int i=0;i<len;i++)
        {
            if(finished==true)break;
            int LayerIDjudge=Points_List.at(i).Layer_ID;//取出一个判断
            if(LayerIDjudge==Layer_ID){//若是正在查找的layerID
                int index_before=0;//在对应PC_ID之前的图元数量
                int PC_id_size=0;//获取当前待查找Pcid拥有的元素数量，以供二分查找
                for(int j=0;j<Layers_List.size();j++){//先找到layer
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){//获取排在待查找PCid之前的索引数量
                        int c=0;
                        for(c=0;c<PC_ID;c++)
                            index_before+=Layers_List.at(j).Every_size[c];
                        PC_id_size=Layers_List.at(j).Every_size[c];
                        break;
                    }
                }
                i+=index_before;

                //二分查找确定值
                int low = i;
                int high = i+PC_id_size-1;
                int mid = 0;
                while ( low <= high )
                {
                    mid = (low + high )/2;

                    if ( Points_List.at(mid).Index_Part < Index_Part)
                        low = mid + 1;
                    else if (Points_List.at(mid).Index_Part > Index_Part )
                        high = mid - 1;
                    else
                    {
                        finished=true;
                        Index_Return=mid;
                        break;
                    }
                }
            }else{//查找layerlist，获取跳过的数量
                int Jump_number=0;
                for(int j=0;j<Layers_List.size();j++){
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        Jump_number=Layers_List.at(j).Size;
                        break;
                    }
                }
                i+=Jump_number;
            }
        }
    }else if(Type==1){
        int len=Lines_List.size();
        bool finished=false;//判断是否停止
        //先找到对应的Layer
        for(int i=0;i<len;i++)
        {
            if(finished==true)break;
            int LayerIDjudge=Lines_List.at(i).Layer_ID;//取出一个判断
            if(LayerIDjudge==Layer_ID){
                int index_before=0;//在对应PC_ID之前的图元数量
                int PC_id_size=0;//获取当前待查找Pcid拥有的元素数量，以供二分查找
                for(int j=0;j<Layers_List.size();j++){//先找到layer
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        for(int c=0;c<PC_ID;c++)
                            index_before+=Layers_List.at(j).Every_size[c];
                        break;
                    }
                }
                i+=index_before;
                //二分查找确定值
                int low = i;
                int high = i+PC_id_size-1;
                int mid = 0;
                while ( low <= high )
                {
                    mid = (low + high )/2;

                    if ( Lines_List.at(mid).Index_Part < Index_Part)
                        low = mid + 1;
                    else if (Lines_List.at(mid).Index_Part > Index_Part )
                        high = mid - 1;
                    else
                    {
                        finished=true;
                        Index_Return=mid;
                        break;
                    }
                }
            }else{//查找layerlist，获取跳过的数量
                int Jump_number=0;
                for(int j=0;j<Layers_List.size();j++){
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        Jump_number=Layers_List.at(j).Size;
                        break;
                    }
                }
                i+=Jump_number;
            }
        }
    }else{
        int len=Polygens_List.size();
        bool finished=false;//判断是否停止
        //先找到对应的Layer
        for(int i=0;i<len;i++)
        {
            if(finished==true)break;
            int LayerIDjudge=Polygens_List.at(i).Layer_ID;//取出一个判断
            if(LayerIDjudge==Layer_ID){
                int index_before=0;//在对应PC_ID之前的图元数量
                int PC_id_size=0;//获取当前待查找Pcid拥有的元素数量，以供二分查找
                for(int j=0;j<Layers_List.size();j++){//先找到layer
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        for(int c=0;c<PC_ID;c++)
                            index_before+=Layers_List.at(j).Every_size[c];
                        break;
                    }
                }
                i+=index_before;
                //二分查找确定值
                int low = i;
                int high = i+PC_id_size-1;
                int mid = 0;
                while ( low <= high )
                {
                    mid = (low + high )/2;

                    if ( Polygens_List.at(mid).Index_Part < Index_Part)
                        low = mid + 1;
                    else if (Lines_List.at(mid).Index_Part > Index_Part )
                        high = mid - 1;
                    else
                    {
                        finished=true;
                        Index_Return=mid;
                        break;
                    }
                }
            }else{//查找layerlist，获取跳过的数量
                int Jump_number=0;
                for(int j=0;j<Layers_List.size();j++){
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        Jump_number=Layers_List.at(j).Size;
                        break;
                    }
                }
                i+=Jump_number;
            }
        }
    }
    return Index_Return;
}
//返回需要添加元素的真实索引值
int Container_List::Add_search(int Layer_ID,int PC_ID,int Type)
{
    int Index_Return=0;//返回为0则表示数组为空
    if(Type==0){
        int len=Points_List.size();
        bool finished=false;//判断是否停止
        //先找到对应的Layer
        for(int i=0;i<len;i++)
        {
            if(finished==true)break;
            int LayerIDjudge=Points_List.at(i).Layer_ID;//取出一个判断
            if(LayerIDjudge==Layer_ID){
                int index_before=0;//在对应PC_ID之前的图元数量
                for(int j=0;j<Layers_List.size();j++){//先找到layer
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        for(int c=0;c<PC_ID+1&&c<10;c++)
                            index_before+=Layers_List.at(j).Every_size[c];
                        break;
                    }
                }
                i+=index_before;
                Index_Return+=i;
                finished=true;
            }else{//查找layerlist，获取跳过的数量
                int Jump_number=0;
                for(int j=0;j<Layers_List.size();j++){
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        Jump_number=Layers_List.at(j).Size;
                        break;
                    }
                }
                i+=Jump_number-1;
            }
        }
    }else if(Type==1){
        int len=Lines_List.size();
        bool finished=false;//判断是否停止
        //先找到对应的Layer
        for(int i=0;i<len;i++)
        {
            if(finished==true)break;
            int LayerIDjudge=Lines_List.at(i).Layer_ID;//取出一个判断
            if(LayerIDjudge==Layer_ID){
                int index_before=0;//在对应PC_ID之前的图元数量
                for(int j=0;j<Layers_List.size();j++){//先找到layer
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        for(int c=0;c<PC_ID+1&&c<10;c++)
                            index_before+=Layers_List.at(j).Every_size[c];
                        break;
                    }
                }
                i+=index_before;
                Index_Return+=i;
                finished=true;
            }else{//查找layerlist，获取跳过的数量
                int Jump_number=0;
                for(int j=0;j<Layers_List.size();j++){
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        Jump_number=Layers_List.at(j).Size;
                        break;
                    }
                }
                i+=Jump_number-1;
            }
        }
    }else{
        int len=Polygens_List.size();
        bool finished=false;//判断是否停止
        //先找到对应的Layer
        for(int i=0;i<len;i++)
        {
            if(finished==true)break;
            int LayerIDjudge=Polygens_List.at(i).Layer_ID;//取出一个判断
            if(LayerIDjudge==Layer_ID){
                int index_before=0;//在对应PC_ID之前的图元数量
                for(int j=0;j<Layers_List.size();j++){//先找到layer
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        for(int c=0;c<PC_ID+1&&c<10;c++)
                            index_before+=Layers_List.at(j).Every_size[c];
                        break;
                    }
                }
                i+=index_before;
                Index_Return+=i;
                finished=true;
            }else{//查找layerlist，获取跳过的数量
                int Jump_number=0;
                for(int j=0;j<Layers_List.size();j++){
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        Jump_number=Layers_List.at(j).Size;
                        break;
                    }
                }
                i+=Jump_number-1;
            }
        }
    }
    return Index_Return;
}
int Container_List::Current_insert(int Layer_ID,int PC_ID,int Index_Part,int Type){
    int Index_Return=-1;//返回
    if(Type==0){
        int len=Points_List.size();
        bool finished=false;//判断是否停止
        //先找到对应的Layer
        for(int i=0;i<len;i++)
        {
            if(finished==true)break;
            int LayerIDjudge=Points_List.at(i).Layer_ID;//取出一个判断
            if(LayerIDjudge==Layer_ID){//若是正在查找的layerID
                int index_before=0;//在对应PC_ID之前的图元数量
                int PC_id_size=0;//获取当前待查找Pcid拥有的元素数量，以供二分查找
                for(int j=0;j<Layers_List.size();j++){//先找到layer
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){//获取排在待查找PCid之前的索引数量
                        int c=0;
                        for(;c<PC_ID;c++)
                            index_before+=Layers_List.at(j).Every_size[c];
                        PC_id_size=Layers_List.at(j).Every_size[c];
                        break;
                    }
                }
                i+=index_before;

                //二分查找确定值
                int low = i;
                int high = i+PC_id_size;
                int mid = 0;
                while ( low <= high )
                {
                    mid = (low + high )/2;
                   if(mid==low||mid==high){
                        if(Points_List.size()<=low+1||(Points_List.at(low).Index_Part<Index_Part&&Points_List.at(low+1).Index_Part>Index_Part))
                        {
                            finished=true;
                            Index_Return=low+1;
                            break;
                        }else{
                            finished=true;
                            Index_Return=len;
                            break;
                        }
                    }
                    if ( Points_List.at(mid).Index_Part < Index_Part)
                        low = mid + 1;
                    else if (Points_List.at(mid).Index_Part > Index_Part )
                        high = mid - 1;
                    else
                    {
                       return -1;//不可能的情况
                    }
                }
            }else{//查找layerlist，获取跳过的数量
                int Jump_number=0;
                for(int j=0;j<Layers_List.size();j++){
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        Jump_number=Layers_List.at(j).Size;
                        break;
                    }
                }
                i+=Jump_number;
            }
        }
    }else if(Type==1){
        int len=Lines_List.size();
        bool finished=false;//判断是否停止
        //先找到对应的Layer
        for(int i=0;i<len;i++)
        {
            if(finished==true)break;
            int LayerIDjudge=Lines_List.at(i).Layer_ID;//取出一个判断
            if(LayerIDjudge==Layer_ID){
                int index_before=0;//在对应PC_ID之前的图元数量
                int PC_id_size=0;//获取当前待查找Pcid拥有的元素数量，以供二分查找
                for(int j=0;j<Layers_List.size();j++){//先找到layer
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        int c=0;
                        for(;c<PC_ID;c++)
                            index_before+=Layers_List.at(j).Every_size[c];
                        PC_id_size=Layers_List.at(j).Every_size[c];
                        break;
                    }
                }
                i+=index_before;
                //二分查找确定值
                int low = i;
                int high = i+PC_id_size-1;
                int mid = 0;
                while ( low <= high )
                {
                    mid = (low + high )/2;
                    if(mid==low||mid==high){
                        if(Points_List.size()<=low+1||Lines_List.at(low).Index_Part<Index_Part&&Lines_List.at(low+1).Index_Part>Index_Part)
                        {
                            finished=true;
                            Index_Return=low+1;
                            break;
                        }else{
                            finished=true;
                            Index_Return=len;
                            break;
                        }
                    }
                    if ( Lines_List.at(mid).Index_Part < Index_Part)
                        low = mid + 1;
                    else if (Lines_List.at(mid).Index_Part > Index_Part )
                        high = mid - 1;
                    else
                    {
                       return -1;//不可能的情况
                    }
                }
            }else{//查找layerlist，获取跳过的数量
                int Jump_number=0;
                for(int j=0;j<Layers_List.size();j++){
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        Jump_number=Layers_List.at(j).Size;
                        break;
                    }
                }
                i+=Jump_number;
            }
        }
    }else{
        int len=Polygens_List.size();
        bool finished=false;//判断是否停止
        //先找到对应的Layer
        for(int i=0;i<len;i++)
        {
            if(finished==true)break;
            int LayerIDjudge=Polygens_List.at(i).Layer_ID;//取出一个判断
            if(LayerIDjudge==Layer_ID){
                int index_before=0;//在对应PC_ID之前的图元数量
                int PC_id_size=0;//获取当前待查找Pcid拥有的元素数量，以供二分查找
                for(int j=0;j<Layers_List.size();j++){//先找到layer
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        int c=0;
                        for(;c<PC_ID;c++)
                            index_before+=Layers_List.at(j).Every_size[c];
                        PC_id_size=Layers_List.at(j).Every_size[c];
                        break;
                    }
                }
                i+=index_before;
                //二分查找确定值
                int low = i;
                int high = i+PC_id_size;
                int mid = 0;
                while ( low <= high )
                {
                    mid = (low + high )/2;
                    if(mid==low||mid==high){

                        if(Points_List.size()<=low+1||Polygens_List.at(low).Index_Part<Index_Part&&Polygens_List.at(low+1).Index_Part>Index_Part)
                        {
                            finished=true;
                            Index_Return=low+1;
                            break;
                        }else{
                            finished=true;
                            Index_Return=len;
                            break;
                        }
                    }
                    if ( Polygens_List.at(mid).Index_Part < Index_Part)
                        low = mid + 1;
                    else if (Polygens_List.at(mid).Index_Part > Index_Part )
                        high = mid - 1;
                    else
                    {
                       return -1;//不可能的情况
                    }
                }
            }else{//查找layerlist，获取跳过的数量
                int Jump_number=0;
                for(int j=0;j<Layers_List.size();j++){
                    if(Layers_List.at(j).Layer_ID==LayerIDjudge){
                        Jump_number=Layers_List.at(j).Size;
                        break;
                    }
                }
                i+=Jump_number;
            }
        }
    }
    return Index_Return;
}
