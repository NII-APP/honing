#include "cvertexes.h"
#include <QVector3D>
namespace CGL {




CVertexes::CVertexes(int size, float val)
    : QVector<float>(size, val)
{
}


QVector3D CVertexes::operator () (int p) const {
    return *static_cast<const QVector3D*>(static_cast<const void*>(this->data() + p * 3));
}




}
