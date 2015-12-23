#include "gdal_readfile.h"

GDAL_ReadFile::GDAL_ReadFile(char* File_name,Container_List &Container_out)
{
    this->File_name=File_name;//保存文件名
    Container=&Container_out;//保存容器引用
    GDALAllRegister();//注册GDAL组件
    Open_Shp();//打开
}
GDAL_ReadFile::~GDAL_ReadFile(){
    GDALClose( poDS );//关闭打开的对象
    OGRFeature::DestroyFeature( poFeature );
}
void GDAL_ReadFile::Open_Shp(){
    poDS= (GDALDataset*) GDALOpenEx( File_name, GDAL_OF_VECTOR, NULL, NULL, NULL );//打开
    if( poDS == NULL )
    {
        printf( "Open failed.\n" );//无法打开则报错
        exit( 1 );
    }
    poLayer = poDS->GetLayer(poDS->GetLayerCount()-1);//打开最后一层
    poLayer->ResetReading();//重置访问，防止出问题
    poFDefn = poLayer->GetLayerDefn();
}

void GDAL_ReadFile::Get_Data(QString Layer_Name){
    int type=-1;
    int size=0;
    while( (poFeature = poLayer->GetNextFeature()) != NULL )//读取几何信息
    {
        size++;
        poGeometry = poFeature->GetGeometryRef();
        poFDefn=poLayer->GetLayerDefn();
        //
        if( poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbPoint )
        {
            OGRPoint *poPoint = (OGRPoint *) poGeometry;
            St_Points temp;
            temp.Point.setX(poPoint->getX());
            temp.Point.setY(poPoint->getY());
            Container->Points_List.append(temp);

//            for(int i = 0;i < poFDefn->GetFieldCount();i++) //列上的
//            {
//                temp.Attribute_Point.append(poFeature->GetFieldAsString(i));
//                Container->Points_List.append(temp);
//            }
            type=0;
        }
        //读取线坐标
        else if( poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbLineString )
        {
            OGRLineString *poLine = (OGRLineString*)poGeometry;
            int a = poLine->getNumPoints();
            St_Lines tempLine;
            for(int i = 0;i <= a; i++){

                QPointF tempPoint;
                tempPoint.setX(poLine->getX(i));
                tempPoint.setY(poLine->getY(i));
                tempLine.Line_FromTo.append(tempPoint);
                Container->Lines_List.append(tempLine);
            }
//            for(int i = 0;i < poFDefn->GetFieldCount();i++) //列上的
//            {
//                tempLine.Attribute_Line.append(poFeature->GetFieldAsString(i));
//                Container->Lines_List.append(tempLine);
//            }
            type=1;
        }
        //读取面坐标(简单多边形，没有内环)
        else if( poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbPolygon)
        {
            OGRPolygon *PoPolygen = (OGRPolygon*)poGeometry;
            OGRLinearRing *ring = PoPolygen->getExteriorRing();//获取该多边形的外环
            St_Polygens tempPolygen;
            for(int i = 0;i <= ring->getNumPoints(); i++){
                QPointF tempPoint;
                //              ring->getPoint(i,&tempPoint);
                qDebug()<<ring->getX(i);
                tempPoint.setX(ring->getX(i));
                tempPoint.setY(ring->getY(i));
                tempPolygen.Polygen_Round.append(tempPoint);
                Container->Polygens_List.append(tempPolygen);
            }
//            for(int i = 0;i < poFDefn->GetFieldCount();i++) //列上的
//            {
//                tempPolygen.Attribute_Polygen.append(poFeature->GetFieldAsString(i));
//                Container->Polygens_List.append(tempPolygen);
//            }
            type=2;
        }
        else
        {
            printf( "no point geometry\n" );
        }
    }
    if(type!=-1){
        Container->Add_Layer(Layer_Name,type);
        Container->Layers_List[Container->Layers_List.size()-1].Size=size;
        Container->Layers_List[Container->Layers_List.size()-1].Every_size[Container->PC_ID]=size;
    }
}
