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
    poFeature = poLayer->GetNextFeature();//打开要素
    poFDefn = poLayer->GetLayerDefn();
    poGeometry = poFeature->GetGeometryRef();
}

void GDAL_ReadFile::Get_Data(){
//    for(int iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
//    {
//        qDebug()<<poFeature->GetFieldAsString( iField );
//    }
    if( poGeometry != NULL
            && wkbFlatten(poGeometry->getGeometryType()) == wkbPoint )
    {
        OGRPoint *poPoint = (OGRPoint *) poGeometry;
        Points temp;
        temp.Point.setX(poPoint->getX());
        temp.Point.setY(poPoint->getY());
        Container->Points_List.append(temp);
        printf( "%.3f,%3.f\n", poPoint->getX(), poPoint->getY() );
    }
    else
    {
        printf( "no point geometry\n" );
    }
}
