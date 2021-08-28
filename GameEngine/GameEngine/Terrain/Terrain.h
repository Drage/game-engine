
#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>
#include "Array2D.h"
#include "Vector3.h"
#include "Renderable.h"

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
        
            float m_maxHeight;
            float m_resolution;
            Array2D<float> m_heightmap;
            Array2D<Vector3> m_normals;
            unsigned m_vao;
            int m_numIndexes;
    };
}

#endif
