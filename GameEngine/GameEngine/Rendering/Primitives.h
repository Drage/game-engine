
#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Renderable.h"

namespace DrageEngine
{
    class Cube : public Renderable
    {
        public:
            void Render() const;
        
        private:
            static void Init();
            static bool init;
            static unsigned vao;
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
            static unsigned vao;
    };

    class Plane : public Renderable
    {
        public:
            void Render() const;
        
        private:
            static void Init();
            static bool init;
            static unsigned vao;
    };

    class Billboard : public Renderable
    {
        public:
            void Render() const;
        
        private:
            static void Init();
            static bool init;
            static unsigned vao;
    };
}

#endif
