
#ifndef MODEL_H
#define MODEL_H

#include <string>
#include "Transform.h"
#include "Mesh.h"
#include "Bounds.h"

namespace DrageEngine
{
    class Model
    {
        public:
            Model();
            virtual ~Model();
        
            virtual bool Load(const std::string &filename);
        
            int GetMeshCount() const;
            Mesh* GetMesh(int index) const;
        
            const std::string& GetName() const;
        
            const Bounds& GetBounds() const;
        
        protected:
            std::string m_name;
            std::vector<Mesh*> m_meshList;
            Bounds m_bounds;
    };
}

#endif
