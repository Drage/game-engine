
#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Mesh.h"

namespace DrageEngine
{
    class Primitive
    {
        public:
            static const Mesh* Cube();
            static const Mesh* Sphere();
            static const Mesh* Plane();
            static const Mesh* Billboard();
        
        private:
            static Mesh *cube;
            static Mesh *sphere;
            static Mesh *plane;
            static Mesh *billboard;
        
            static const int SPHERE_SLICES;
            static const int SPHERE_STACKS;
    };
}

#endif
