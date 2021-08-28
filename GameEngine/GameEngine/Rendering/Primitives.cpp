
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include <vector>
#include <cmath>
#include "Primitives.h"
#include "Vector3.h"
#include "Vector2.h"
#include "MathUtils.h"

using namespace DrageEngine;

bool Cube::init = false;
unsigned Cube::vao;

void Cube::Init()
{
    GLfloat vertexData[] = {
        // X    Y     Z       U     V          Normal
        // Bottom
        -0.5f,-0.5f,-0.5f,   0.0f, 0.0f,   0.0f,-1.0f, 0.0f,
         0.5f,-0.5f,-0.5f,   1.0f, 0.0f,   0.0f,-1.0f, 0.0f,
        -0.5f,-0.5f, 0.5f,   0.0f, 1.0f,   0.0f,-1.0f, 0.0f,
         0.5f,-0.5f,-0.5f,   1.0f, 0.0f,   0.0f,-1.0f, 0.0f,
         0.5f,-0.5f, 0.5f,   1.0f, 1.0f,   0.0f,-1.0f, 0.0f,
        -0.5f,-0.5f, 0.5f,   0.0f, 1.0f,   0.0f,-1.0f, 0.0f,
        
        // Top
        -0.5f, 0.5f,-0.5f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
         0.5f, 0.5f,-0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
         0.5f, 0.5f,-0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        
        // Front
        -0.5f,-0.5f, 0.5f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         0.5f,-0.5f, 0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
         0.5f,-0.5f, 0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        
        // Back
        -0.5f,-0.5f,-0.5f,   0.0f, 0.0f,   0.0f, 0.0f,-1.0f,
        -0.5f, 0.5f,-0.5f,   0.0f, 1.0f,   0.0f, 0.0f,-1.0f,
         0.5f,-0.5f,-0.5f,   1.0f, 0.0f,   0.0f, 0.0f,-1.0f,
         0.5f,-0.5f,-0.5f,   1.0f, 0.0f,   0.0f, 0.0f,-1.0f,
        -0.5f, 0.5f,-0.5f,   0.0f, 1.0f,   0.0f, 0.0f,-1.0f,
         0.5f, 0.5f,-0.5f,   1.0f, 1.0f,   0.0f, 0.0f,-1.0f,
        
        // Left
        -0.5f,-0.5f, 0.5f,   0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,   1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f,-0.5f,-0.5f,   0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f,-0.5f, 0.5f,   0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,   1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,   1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        
        // Right
         0.5f,-0.5f, 0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         0.5f,-0.5f,-0.5f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.5f, 0.5f,-0.5f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.5f,-0.5f, 0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         0.5f, 0.5f,-0.5f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f
    };
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat)));
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    init = true;
}

void Cube::Render() const
{
    if (!init)
        Init();
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);
    glBindVertexArray(0);
}


bool Sphere::init = false;
unsigned Sphere::vao;
const int Sphere::SLICES = 64;
const int Sphere::STACKS = 64;

void Sphere::Init()
{
    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> textureCoords;
    
    for (int i = 0; i <= STACKS; i++)
    {
        float v = i / (float)STACKS;
        float phi = v * PI;

        for (int j = 0; j <= SLICES; j++)
        {
            float u = j / (float)SLICES;
            float theta = u * PI * 2;

            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            positions.push_back(Vector3(x, y, z));
            normals.push_back(Vector3(x, y, z));
            textureCoords.insert(textureCoords.begin(), Vector2(u, v));
        }
    }
    
    std::vector<GLuint> indicies;
    for (int i = 0; i < SLICES * STACKS + SLICES; i++)
    {
        indicies.push_back(i);
        indicies.push_back(i + SLICES + 1);
        indicies.push_back(i + SLICES);
    
        indicies.push_back(i + SLICES + 1);
        indicies.push_back(i);
        indicies.push_back(i + 1);
    }
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint vbos[4];
    glGenBuffers(4, vbos);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(Vector3), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(Vector2), textureCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vector3), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(GLuint), indicies.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    init = true;
}

void Sphere::Render() const
{
    if (!init)
        Init();
    
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (SLICES * STACKS + SLICES) * 6, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

bool Plane::init = false;
unsigned Plane::vao;

void Plane::Init()
{
    GLfloat vertexData[] = {
        // X    Y     Z       U     V          Normal
        -0.5f, 0.0f,-0.5f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
         0.5f, 0.0f,-0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
         0.5f, 0.0f,-0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
         0.5f, 0.0f, 0.5f,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
    };
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat)));
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    init = true;
}

void Plane::Render() const
{
    if (!init)
        Init();
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

bool Billboard::init = false;
unsigned Billboard::vao;

void Billboard::Init()
{
    GLfloat vertexData[] = {
        // X    Y     Z       U     V
         0.5f, 0.5f, 0.0f,   1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f,   0.0f, 1.0f,
         0.5f,-0.5f, 0.0f,   1.0f, 0.0f,
         0.5f,-0.5f, 0.0f,   1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,   0.0f, 1.0f,
        -0.5f,-0.5f, 0.0f,   0.0f, 0.0f,
    };
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    init = true;
}

void Billboard::Render() const
{
    if (!init)
        Init();
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

