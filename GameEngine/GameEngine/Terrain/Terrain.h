
#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>
#include "Array2D.h"
#include "Vector3.h"
#include "Renderable.h"
#include "Mesh.h"

namespace DrageEngine
{
    class Terrain : public Renderable
    {
        public:
            Terrain();
            Terrain(const std::string &heightmap, float maxHeight = 10, float resolution = 1);
            ~Terrain();
        
            bool Load(const std::string &heightmap, float maxHeight = 10, float resolution = 1);
        
            float HeightAt(const Vector3 &worldPosition) const;
            float HeightAt(float x, float z) const;
        
            Vector3 NormalAt(const Vector3 &worldPosition) const;
            Vector3 NormalAt(float x, float z) const;
        
            void Render() const;
        
        private:
            float GetHeightFromPixelData(unsigned char* data, unsigned bpp) const;
            void InitRenderBuffers();
        
            float maxHeight;
            float resolution;
            Array2D<float> heightmap;
            Array2D<Vector3> normals;
            Mesh mesh;
    };
}

#endif
