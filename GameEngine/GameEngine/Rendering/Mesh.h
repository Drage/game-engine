
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "Vertex.h"
#include "Material.h"
#include "Bounds.h"

namespace DrageEngine
{
    class Mesh
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
                GenerateExtendedBuffer(extData);
            }
        
            unsigned GetID() const;
            const std::string& GetName() const;
        
            Material* GetMaterial() const;
            void SetMaterial(Material *material);
        
            bool HasIndices() const;
            int GetVertexCount() const;
            int GetIndexCount() const;
            Bounds GetBounds() const;
        
        private:
            template<class T>
            void GenerateExtendedBuffer(const std::vector<T> &extData);
            void CalculateBounds(const std::vector<Vertex> &vertices);
        
            unsigned vao;
            unsigned verticesVbo;
            unsigned indicesVbo;
            unsigned extendedVbo;
            int vertexCount;
            int indexCount;
            std::string name;
            Material *material;
            Bounds bounds;
    };
}

#endif
