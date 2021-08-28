
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
            Mesh(const std::string &name, const std::vector<Vertex> &vertices);
            Mesh(const std::vector<Vertex> &vertices);
            ~Mesh();

            unsigned Generate(const std::vector<Vertex> &vertices);
        
            unsigned GetID() const;
            const std::string& GetName() const;
        
            int GetVertexCount() const;
        
            Material* GetMaterial() const;
            void SetMaterial(Material *material);
        
            void Render() const;
        
        private:
            unsigned vao;
            unsigned vbo;
            std::string name;
            int vertexCount;
            Material *material;
    };
}

#endif
