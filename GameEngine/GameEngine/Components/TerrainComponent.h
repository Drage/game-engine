
#ifndef TERRAINCOMPONENT_H
#define TERRAINCOMPONENT_H

#include <string>
#include "Component.h"
#include "Terrain.h"
#include "Material.h"
#include "Renderable.h"

namespace DrageEngine
{
    class TerrainComponent : public Component, public Terrain
    {
        public:
            void Init(const ParamList &params);
            ~TerrainComponent();
        
        private:
            Material *material;
            bool disableDepthWrite;
            Renderable *renderable;
    };
}

#endif
