
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "Vertex.h"
#include "Material.h"
#include "Renderable.h"

namespace DrageEngine
{
    class Mesh : public Renderable
    {
        public:
            Mesh();
            Mesh(const std::string &name);
            ~Mesh();

            unsigned Generate(const std::vector<Vertex> &vertices);
            unsigned Generate(const std::vector<Vertex> &vertices, const std::vector<int> &indices);
        
            unsigned GetID() const;
            const std::string& GetName() const;
        
            Material* GetMaterial() const;
            void SetMaterial(Material *material);
        
            void Render() const;
        
        private:
            unsigned vao;
            unsigned verticesVbo;
            unsigned indicesVbo;
            int vertexCount;
            int indexCount;
            std::string name;
            Material *material;
    };
}

#endif
