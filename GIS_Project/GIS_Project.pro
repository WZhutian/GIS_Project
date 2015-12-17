#-------------------------------------------------
#
# Project created by QtCreator 2015-12-08T20:52:37
#
#-------------------------------------------------

QT       += core gui network
CONFIG   -= app_bundle
CONFIG   += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GIS_Project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    container_list.cpp \
    gdal_readfile.cpp \
    editwidget.cpp \
    tcpserver.cpp \
    tcpsocket.cpp \
    threadhandle.cpp

HEADERS  += mainwindow.h \
    GDAL_include/cpl_atomic_ops.h \
    GDAL_include/cpl_config.h \
    GDAL_include/cpl_config_extras.h \
    GDAL_include/cpl_conv.h \
    GDAL_include/cpl_csv.h \
    GDAL_include/cpl_error.h \
    GDAL_include/cpl_hash_set.h \
    GDAL_include/cpl_http.h \
    GDAL_include/cpl_list.h \
    GDAL_include/cpl_minixml.h \
    GDAL_include/cpl_minizip_ioapi.h \
    GDAL_include/cpl_minizip_unzip.h \
    GDAL_include/cpl_minizip_zip.h \
    GDAL_include/cpl_multiproc.h \
    GDAL_include/cpl_odbc.h \
    GDAL_include/cpl_port.h \
    GDAL_include/cpl_progress.h \
    GDAL_include/cpl_quad_tree.h \
    GDAL_include/cpl_spawn.h \
    GDAL_include/cpl_string.h \
    GDAL_include/cpl_time.h \
    GDAL_include/cpl_virtualmem.h \
    GDAL_include/cpl_vsi.h \
    GDAL_include/cpl_vsi_virtual.h \
    GDAL_include/cpl_vsil_curl_priv.h \
    GDAL_include/cpl_win32ce_api.h \
    GDAL_include/cpl_wince.h \
    GDAL_include/cplkeywordparser.h \
    GDAL_include/gdal.h \
    GDAL_include/gdal_alg.h \
    GDAL_include/gdal_alg_priv.h \
    GDAL_include/gdal_csv.h \
    GDAL_include/gdal_frmts.h \
    GDAL_include/gdal_mdreader.h \
    GDAL_include/gdal_pam.h \
    GDAL_include/gdal_priv.h \
    GDAL_include/gdal_proxy.h \
    GDAL_include/gdal_rat.h \
    GDAL_include/gdal_simplesurf.h \
    GDAL_include/gdal_version.h \
    GDAL_include/gdal_vrt.h \
    GDAL_include/gdalexif.h \
    GDAL_include/gdalgeorefpamdataset.h \
    GDAL_include/gdalgrid.h \
    GDAL_include/gdalgrid_priv.h \
    GDAL_include/gdaljp2abstractdataset.h \
    GDAL_include/gdaljp2metadata.h \
    GDAL_include/gdaljp2metadatagenerator.h \
    GDAL_include/gdalsse_priv.h \
    GDAL_include/gdalwarper.h \
    GDAL_include/gdalwarpkernel_opencl.h \
    GDAL_include/gvgcpfit.h \
    GDAL_include/memdataset.h \
    GDAL_include/ogr_api.h \
    GDAL_include/ogr_core.h \
    GDAL_include/ogr_feature.h \
    GDAL_include/ogr_featurestyle.h \
    GDAL_include/ogr_geocoding.h \
    GDAL_include/ogr_geometry.h \
    GDAL_include/ogr_p.h \
    GDAL_include/ogr_spatialref.h \
    GDAL_include/ogr_srs_api.h \
    GDAL_include/ogrsf_frmts.h \
    GDAL_include/rawdataset.h \
    GDAL_include/thinplatespline.h \
    GDAL_include/vrtdataset.h \
    container_list.h \
    gdal_readfile.h \
    struct_list.h \
    mydatastream.h \
    editwidget.h \
    tcpserver.h \
    tcpsocket.h \
    threadhandle.h

FORMS    += mainwindow.ui

LIBS += D:\GIS_Project\GIS_Project\GIS_Project\DLLs\libgdal.dll

DISTFILES += \
    DLLs/libgdal.dll \
    ICON/background.png \
    ICON/canvas.png \
    ICON/circle.png \
    ICON/copy.png \
    ICON/cut.png \
    ICON/delete.png \
    ICON/edit.png \
    ICON/ellipse.png \
    ICON/eraser.png \
    ICON/exit.png \
    ICON/fill.png \
    ICON/find.png \
    ICON/foreground.png \
    ICON/fullExtent.png \
    ICON/line.png \
    ICON/move.png \
    ICON/new.png \
    ICON/open.png \
    ICON/paint.png \
    ICON/pan.png \
    ICON/paste.png \
    ICON/path.png \
    ICON/pencil.png \
    ICON/point.png \
    ICON/polygon.png \
    ICON/polyline.png \
    ICON/property.png \
    ICON/qt.png \
    ICON/rectangle.png \
    ICON/redo.png \
    ICON/save.png \
    ICON/saveAs.png \
    ICON/select.png \
    ICON/selectAll.png \
    ICON/text.png \
    ICON/undo.png \
    ICON/unselect.png \
    ICON/unselectAll.png \
    ICON/zoomIn.png \
    ICON/zoomOut.png \
    ICON/canvas.ico \
    ICON/layer_add.ico \
    ICON/NewShape.ico \
    ICON/paint.ico \
    ICON/save.ico \
    ICON/scene.ico \
    ICON/zoom_in.ico \
    ICON/zoom_out.ico

RESOURCES += \
    ICON/icon.qrc
