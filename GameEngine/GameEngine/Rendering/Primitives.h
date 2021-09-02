
#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Renderable.h"
#include "Mesh.h"

namespace DrageEngine
{
    class Cube : public Renderable
    {
        public:
            void Render() const;
        
        private:
            static void Init();
            static bool init;
            static Mesh mesh;
    };
    
    class Sphere : public Renderable
    {
        public:
            void Render() const;
        
        private:
            static void Init();
            static const int SLICES;
            static const int STACKS;
            static bool init;
            static Mesh mesh;
    };

    class Plane : public Renderable
    {
        public:
            void Render() const;
        
        private:
            static void Init();
            static bool init;
            static Mesh mesh;
    };

    class Billboard : public Renderable
    {
        public:
            void Render() const;
        
        private:
            static void Init();
            static bool init;
            static Mesh mesh;
    };
}

#endif
