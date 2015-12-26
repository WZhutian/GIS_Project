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

    poFeature = poLayer->GetFeature(0);//打开要素
    poFDefn = poLayer->GetLayerDefn();
    poGeometry = poFeature->GetGeometryRef();
}

void GDAL_ReadFile::Get_Data(QString Layer_Name){
    QList<QString> temp_attr;
    for(int i = 0;i < poFDefn->GetFieldCount();i++){
        poGeometry = poFeature->GetGeometryRef();
        OGRFieldDefn *pFieldDefn = poFDefn->GetFieldDefn(i);
        poGeometry->getGeometryType();
        temp_attr.append(pFieldDefn->GetNameRef());
    }
    bool add_layer=false;
    while( (poFeature = poLayer->GetNextFeature()) != NULL )//读取几何信息
    {
        poGeometry = poFeature->GetGeometryRef();
        poFDefn=poLayer->GetLayerDefn();
        //
        if( poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbPoint )
        {
            if(add_layer==false){
                Container->Add_Layer(Layer_Name,0);
                Container->Layers_List[Container->Layers_List.size()-1].Attribute_Name=temp_attr;
                add_layer=true;
            }
            OGRPoint *poPoint = (OGRPoint *) poGeometry;
            QList<QString> Attr;
            QPointF temp;
            temp.setX(poPoint->getX());
            temp.setY(poPoint->getY());
            int restore_temp=Container->Layer_ID;
            Container->Layer_ID=Container->Layers_List.size()-1;
            Container->Add_Point(temp);
            Container->Layer_ID=restore_temp;
            for(int i = 0;i < poFDefn->GetFieldCount();i++) //列上的
            {
                Attr.append(poFeature->GetFieldAsString(i));
            }
            Container->Points_List[Container->Points_List.size()-1].Attribute_Point=Attr;
         }
        //读取线坐标
        else if( poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbLineString )
        {
            if(add_layer==false){
                Container->Add_Layer(Layer_Name,0);
                Container->Layers_List[Container->Layers_List.size()-1].Attribute_Name=temp_attr;
                add_layer=true;
            }
            OGRLineString *poLine = (OGRLineString*)poGeometry;
            int a = poLine->getNumPoints();
            QVector<QPointF> tempLine;
            QList<QString> Attr;
            int restore_temp=Container->Layer_ID;
            Container->Layer_ID=Container->Layers_List.size()-1;
            for(int i = 0;i <= a; i++){
                QPointF tempPoint;
                tempPoint.setX(poLine->getX(i));
                tempPoint.setY(poLine->getY(i));
                tempLine.append(tempPoint);
            }
            Container->Add_Line(tempLine);
            Container->Layer_ID=restore_temp;
            for(int i = 0;i < poFDefn->GetFieldCount();i++) //列上的
            {
                Attr.append(poFeature->GetFieldAsString(i));
            }
            Container->Lines_List[Container->Lines_List.size()-1].Attribute_Line=Attr;
        }
        //读取面坐标(简单多边形，没有内环)
        else if( poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbPolygon)
        {
            if(add_layer==false){
                Container->Add_Layer(Layer_Name,0);
                Container->Layers_List[Container->Layers_List.size()-1].Attribute_Name=temp_attr;
                add_layer=true;
            }
            OGRPolygon *PoPolygen = (OGRPolygon*)poGeometry;
            OGRLinearRing *ring = PoPolygen->getExteriorRing();//获取该多边形的外环
            QVector<QPointF> tempPolygen;
            QList<QString> Attr;
            int restore_temp=Container->Layer_ID;
            Container->Layer_ID=Container->Layers_List.size()-1;
            for(int i = 0;i <= ring->getNumPoints(); i++){
                QPointF tempPoint;
                //              ring->getPoint(i,&tempPoint);
                tempPoint.setX(ring->getX(i));
                tempPoint.setY(ring->getY(i));
                tempPolygen.append(tempPoint);
            }
            Container->Add_Polygen(tempPolygen);
            Container->Layer_ID=restore_temp;
            for(int i = 0;i < poFDefn->GetFieldCount();i++) //列上的
            {
                Attr.append(poFeature->GetFieldAsString(i));
            }
            Container->Polygens_List[Container->Polygens_List.size()-1].Attribute_Polygen=Attr;
        }
        else
        {
            printf( "no point geometry\n" );
        }
    }

}
