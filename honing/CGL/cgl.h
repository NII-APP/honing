#ifndef CGL_H
#define CGL_H

#include <QVector>
#include <QFont>
#include "cvertexes.h"

namespace CGL
{
typedef CVertexes Vertexes;
typedef QVector<unsigned int> Trace;
typedef QVector<unsigned char> Colors;
typedef QVector<float*> Matrix;

const QFont DEFAULT_FONT("Cambria Math, Times New Roman, Times, Serif", 10, 0);
}

#endif // CGL_H
