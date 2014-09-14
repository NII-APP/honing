#ifndef VERTEXES_H
#define VERTEXES_H
#include <QVector>
class QVector3D;

namespace CGL {
class CVertexes : QVector<float>
{
public:
    CVertexes(int size, float val = 0.0f);
    QVector3D operator () (int p) const;
};
}

#endif // VERTEXES_H
