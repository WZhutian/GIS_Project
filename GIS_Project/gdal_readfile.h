#ifndef GDAL_READFILE_H
#define GDAL_READFILE_H

#include "GDAL_include/ogrsf_frmts.h"
#include "GDAL_include/gdal_priv.h"
#include "GDAL_include/cpl_conv.h"
#include <container_list.h>
#include <QDebug>
#include <QPointF>
class GDAL_ReadFile
{
public:
    GDAL_ReadFile(char* File_name,Container_List &Container_out);//构造函数，传入文件名，以及容器
    ~GDAL_ReadFile();
    void Open_Shp();
    void Get_Data();//将shp中的数据传入引用参数容器中
private:
    Container_List *Container;
    char* File_name;//保存要读取的文件名
    OGRGeometry *poGeometry;//保存
    GDALDataset *poDS;//已经打开的shp格式文件对象
    OGRFeature *poFeature;//要素对象
    OGRLayer  *poLayer;//图层对象
    OGRFeatureDefn *poFDefn;//要素集信息，包含架构信息
};

#endif // GDAL_READFILE_H
