#ifndef MYDATASTREAM
#define MYDATASTREAM
#include <QDataStream>
#include <container_list.h>
//传单个点
inline QDataStream& operator<<(QDataStream& out, const St_Points& Points)
{
    out<<Points.Point<<Points.Attribute_Point<<Points.Layer_ID<<Points.PC_ID<<
         Points.Index_Part;
    return out;
}
inline QDataStream& operator>>(QDataStream& in,St_Points& Points)
{
    in>>Points.Point>>Points.Attribute_Point>>Points.Layer_ID>>Points.PC_ID>>
            Points.Index_Part;
    return in;
}
inline QDataStream& operator<<(QDataStream& out, const St_Lines& Lines)
{
    out<<Lines.Line_FromTo<<Lines.Attribute_Line<<Lines.Layer_ID<<Lines.PC_ID<<
         Lines.Index_Part;
    return out;
}
inline QDataStream& operator>>(QDataStream& in,St_Lines& Lines)
{
    in>>Lines.Line_FromTo>>Lines.Attribute_Line>>Lines.Layer_ID>>Lines.PC_ID>>
                           Lines.Index_Part;
    return in;
}
inline QDataStream& operator<<(QDataStream& out, const St_Polygens& Polygens)
{
    out<<Polygens.Polygen_Round<<Polygens.Attribute_Polygen<<Polygens.Layer_ID<<Polygens.PC_ID<<
         Polygens.Index_Part;
    return out;
}
inline QDataStream& operator>>(QDataStream& in,St_Polygens& Polygens)
{
    in>>Polygens.Polygen_Round>>Polygens.Attribute_Polygen>>Polygens.Layer_ID>>Polygens.PC_ID>>
            Polygens.Index_Part;
    return in;
}
inline QDataStream& operator<<(QDataStream& out, const St_Layers& Layers)
{
    out<<Layers.Layer_ID<<Layers.Layer_Name<<Layers.Pc_numbers;
    for(int i=0;i<10;i++){
        out<<Layers.Every_size[i];
    }
    out<<Layers.Attribute_Name;
    out<<Layers.Size<<Layers.Ob_Type;
    out<<Layers.PC_ID<<Layers.Index_Part<<Layers.Change_Way<<Layers.Accept_PC;

    return out;
}
inline QDataStream& operator>>(QDataStream& in,St_Layers& Layers)
{
    in>>Layers.Layer_ID>>Layers.Layer_Name>>Layers.Pc_numbers;
    for(int i=0;i<10;i++){
        in>>Layers.Every_size[i];
    }
    in>>Layers.Attribute_Name;
    in>>Layers.Size>>Layers.Ob_Type;
    in>>Layers.PC_ID>>Layers.Index_Part>>Layers.Change_Way>>Layers.Accept_PC;

    return in;
}
#endif // MYDATASTREAM

