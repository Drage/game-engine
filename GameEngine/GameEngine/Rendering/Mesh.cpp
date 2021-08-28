
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include "Mesh.h"

using namespace DrageEngine;

Mesh::Mesh()
{
    vao = 0;
    vbo = 0;
    vertexCount = 0;
    material = NULL;
}

Mesh::Mesh(const std::string &meshName)
{
    vao = 0;
    vbo = 0;
    name = meshName;
    vertexCount = 0;
    material = NULL;
}

Mesh::Mesh(const std::string &meshName, const std::vector<Vertex> &vertices)
{
    vao = 0;
    vbo = 0;
    name = meshName;
    vertexCount = 0;
    material = NULL;
    Generate(vertices);
}

Mesh::Mesh(const std::vector<Vertex> &vertices)
{
    vao = 0;
    vbo = 0;
    vertexCount = 0;
    material = NULL;
    Generate(vertices);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

unsigned Mesh::Generate(const std::vector<Vertex> &vertices)
{
    vertexCount = (int)vertices.size();
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3 * sizeof(GLfloat)));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(5 * sizeof(GLfloat)));
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    return vao;
}

unsigned Mesh::GetID() const
{
    return vao;
}

const std::string& Mesh::GetName() const
{
    return name;
}

int Mesh::GetVertexCount() const
{
    return vertexCount;
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
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}
