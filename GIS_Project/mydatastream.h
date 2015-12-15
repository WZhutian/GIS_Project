#ifndef MYDATASTREAM
#define MYDATASTREAM
#include <QDataStream>
#include <container_list.h>
//传单个点
inline QDataStream& operator<<(QDataStream& out, const St_Points& Points)
{
    out<<Points.Point<<Points.Attribute_Point<<Points.Layer_ID;
    return out;
}
inline QDataStream& operator>>(QDataStream& in,St_Points& Points)
{
    in>>Points.Point>>Points.Attribute_Point>>Points.Layer_ID;
    return in;
}
#endif // MYDATASTREAM

