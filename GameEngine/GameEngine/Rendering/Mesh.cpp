
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include "Mesh.h"

using namespace DrageEngine;

Mesh::Mesh()
{
    m_vao = 0;
    m_vbo = 0;
    m_vertexCount = 0;
    m_material = NULL;
}

Mesh::Mesh(const std::string &name)
{
    m_vao = 0;
    m_vbo = 0;
    m_name = name;
    m_vertexCount = 0;
    m_material = NULL;
}

Mesh::Mesh(const std::string &name, const std::vector<Vertex> &vertices)
{
    m_vao = 0;
    m_vbo = 0;
    m_name = name;
    m_vertexCount = 0;
    m_material = NULL;
    Generate(vertices);
}

Mesh::Mesh(const std::vector<Vertex> &vertices)
{
    m_vao = 0;
    m_vbo = 0;
    m_vertexCount = 0;
    m_material = NULL;
    Generate(vertices);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

unsigned Mesh::Generate(const std::vector<Vertex> &vertices)
{
    m_vertexCount = (int)vertices.size();
    
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3 * sizeof(GLfloat)));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(5 * sizeof(GLfloat)));
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    return m_vao;
}

unsigned Mesh::GetID() const
{
    return m_vao;
}

const std::string& Mesh::GetName() const
{
    return m_name;
}

int Mesh::GetVertexCount() const
{
    return m_vertexCount;
}

Material* Mesh::GetMaterial() const
{
    return m_material;
}

void Mesh::SetMaterial(Material *material)
{
    m_material = material;
}

void Mesh::Render() const
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}
