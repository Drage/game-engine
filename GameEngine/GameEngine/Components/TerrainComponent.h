
#ifndef TERRAINCOMPONENT_H
#define TERRAINCOMPONENT_H

#include <string>
#include "Component.h"
#include "Terrain.h"
#include "Material.h"

namespace DrageEngine
{
    class TerrainComponent : public Component, public Terrain
    {
        public:
            void Init(const ParamList &params);
            void Render(Renderer *renderer, const Transform *transform = NULL) const;
        
        private:
            Material *material;
            bool disableDepthWrite;
    };
}

#endif
