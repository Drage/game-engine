
#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Entity.h"

namespace DrageEngine
{
    class Renderable
    {
        public:
            Renderable(const Mesh *mesh, const Material *material, Entity *entity, unsigned options = 0);
            ~Renderable();
        
            const Mesh* GetMesh() const;
            const Material* GetMaterial() const;
            Entity* GetEntity() const;
            unsigned GetOptions() const;
        
            void SetDepth(float depth);
            float GetDepth() const;
        
            unsigned long GetSortKey() const;
        
        private:
            const Mesh *mesh;
            const Material *material;
            Entity *entity;
            float depth;
            unsigned long sortKey;
            unsigned options;
            
    };

    struct RenderableComparitor
    {
        bool operator()(const Renderable* lhs, const Renderable* rhs) const
        {
            return lhs->GetSortKey() > rhs->GetSortKey();
        }
    };
}

#endif
