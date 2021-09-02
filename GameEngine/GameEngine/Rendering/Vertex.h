
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
        
        Vertex() {}
        
        Vertex(const Vector3 &position, const Vector2 &texCoord, const Vector3 &normal)
        {
            this->position = position;
            this->texCoord = texCoord;
            this->normal = normal;
        }
        
        Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
        {
            this->position = Vector3(x, y, z);
            this->texCoord = Vector2(u, v);
            this->normal = Vector3(nx, ny, nz);
        }
    };
}

#endif
