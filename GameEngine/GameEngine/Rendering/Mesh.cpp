
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include <limits>
#include "Mesh.h"
#include "Debug.h"

using namespace DrageEngine;

Mesh::Mesh()
{
    vao = 0;
    verticesVbo = 0;
    indicesVbo = 0;
    vertexCount = 0;
    indexCount = 0;
    material = NULL;
}

Mesh::Mesh(const std::string &meshName)
{
    vao = 0;
    verticesVbo = 0;
    indicesVbo = 0;
    vertexCount = 0;
    indexCount = 0;
    material = NULL;
    name = meshName;
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &verticesVbo);
    glDeleteBuffers(1, &verticesVbo);
    glDeleteVertexArrays(1, &vao);
}

void Mesh::Generate(const std::vector<Vertex> &vertices)
{
    vertexCount = (int)vertices.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &verticesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3 * sizeof(GLfloat)));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(5 * sizeof(GLfloat)));
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    CalculateBounds(vertices);
}

void Mesh::Generate(const std::vector<Vertex> &vertices, const std::vector<unsigned> &indices)
{
    Generate(vertices);
    indexCount = (int)indices.size();

    glBindVertexArray(vao);

    glGenBuffers(1, &indicesVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

template <class T>
void Mesh::GenerateExtendedBuffer(const std::vector<T> &extData)
{
    glBindVertexArray(vao);
    glGenBuffers(1, &extendedVbo);
    glBindBuffer(GL_ARRAY_BUFFER, extendedVbo);
    glBufferData(GL_ARRAY_BUFFER, extData.size() * sizeof(T), extData.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, sizeof(T) / sizeof(float), GL_FLOAT, GL_FALSE, 0, NULL);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
template void Mesh::GenerateExtendedBuffer<float>(const std::vector<float> &extData);
template void Mesh::GenerateExtendedBuffer<Vector2>(const std::vector<Vector2> &extData);
template void Mesh::GenerateExtendedBuffer<Vector3>(const std::vector<Vector3> &extData);

void Mesh::CalculateBounds(const std::vector<Vertex> &vertices)
{
    auto x = std::minmax_element(vertices.begin(), vertices.end(), [](const Vertex& a, const Vertex& b) { return a.position.x < b.position.x; });
    auto y = std::minmax_element(vertices.begin(), vertices.end(), [](const Vertex& a, const Vertex& b) { return a.position.y < b.position.y; });
    auto z = std::minmax_element(vertices.begin(), vertices.end(), [](const Vertex& a, const Vertex& b) { return a.position.z < b.position.z; });
    bounds.min = Vector3(x.first->position.x, y.first->position.y, z.first->position.z);
    bounds.max = Vector3(x.second->position.x, y.second->position.y, z.second->position.z);
    
    auto r = std::max_element(vertices.begin(), vertices.end(), [](const Vertex& a, const Vertex& b) {
        return a.position.MagnitudeSqr() < b.position.MagnitudeSqr();
    });
    bounds.radius = r->position.Magnitude();
}

unsigned Mesh::GetID() const
{
    return vao;
}

const std::string& Mesh::GetName() const
{
    return name;
}

Material* Mesh::GetMaterial() const
{
    return material;
}

void Mesh::SetMaterial(Material *material)
{
    this->material = material;
}

bool Mesh::HasIndices() const
{
    return indexCount > 0;
}

int Mesh::GetVertexCount() const
{
    return vertexCount;
}

int Mesh::GetIndexCount() const
{
    return indexCount;
}

Bounds Mesh::GetBounds() const
{
    return bounds;
}
