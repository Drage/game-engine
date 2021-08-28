
#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include "Vector3.h"
#include "Vector2.h"

namespace DrageEngine
{
    struct Vertex
    {
        Vector3 position;
        Vector2 texCoord;
        Vector3 normal;
    };
}

#endif
