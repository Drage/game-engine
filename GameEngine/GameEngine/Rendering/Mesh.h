
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

            void Generate(const std::vector<Vertex> &vertices);
            void Generate(const std::vector<Vertex> &vertices, const std::vector<unsigned> &indices);
        
            template<class T>
            void Generate(const std::vector<Vertex> &vertices, const std::vector<unsigned> &indices, const std::vector<T> &extData)
            {
                Generate(vertices, indices);
                GenerateExtentedBuffer(extData);
            }
        
            unsigned GetID() const;
            const std::string& GetName() const;
        
            Material* GetMaterial() const;
            void SetMaterial(Material *material);
        
            void Render() const;
        
        private:
            template<class T>
            void GenerateExtentedBuffer(const std::vector<T> &extData);
        
            unsigned vao;
            unsigned verticesVbo;
            unsigned indicesVbo;
            unsigned extendedVbo;
            int vertexCount;
            int indexCount;
            std::string name;
            Material *material;
    };
}

#endif
