
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include "Mesh.h"

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
void Mesh::GenerateExtentedBuffer(const std::vector<T> &extData)
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
template void Mesh::GenerateExtentedBuffer<float>(const std::vector<float> &extData);
template void Mesh::GenerateExtentedBuffer<Vector2>(const std::vector<Vector2> &extData);
template void Mesh::GenerateExtentedBuffer<Vector3>(const std::vector<Vector3> &extData);

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

void Mesh::Render() const
{
    glBindVertexArray(vao);
    
    if (indexCount != 0)
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);
    else
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    
    glBindVertexArray(0);
}
