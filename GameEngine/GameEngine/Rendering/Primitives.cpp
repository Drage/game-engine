
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
Mesh Cube::mesh;

void Cube::Init()
{
    std::vector<Vertex> vertices
    {
        //        X    Y     Z       U     V           Normal
        // Bottom
        Vertex(-0.5f,-0.5f,-0.5f,   0.0f, 0.0f,   0.0f,-1.0f, 0.0f),
        Vertex( 0.5f,-0.5f,-0.5f,   1.0f, 0.0f,   0.0f,-1.0f, 0.0f),
        Vertex(-0.5f,-0.5f, 0.5f,   0.0f, 1.0f,   0.0f,-1.0f, 0.0f),
        Vertex( 0.5f,-0.5f,-0.5f,   1.0f, 0.0f,   0.0f,-1.0f, 0.0f),
        Vertex( 0.5f,-0.5f, 0.5f,   1.0f, 1.0f,   0.0f,-1.0f, 0.0f),
        Vertex(-0.5f,-0.5f, 0.5f,   0.0f, 1.0f,   0.0f,-1.0f, 0.0f),

        // Top
        Vertex(-0.5f, 0.5f,-0.5f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f),
        Vertex(-0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f),
        Vertex( 0.5f, 0.5f,-0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f),
        Vertex( 0.5f, 0.5f,-0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f),
        Vertex(-0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f),
        Vertex( 0.5f, 0.5f, 0.5f,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f),

        // Front
        Vertex(-0.5f,-0.5f, 0.5f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f),
        Vertex( 0.5f,-0.5f, 0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f),
        Vertex(-0.5f, 0.5f, 0.5f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f),
        Vertex( 0.5f,-0.5f, 0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f),
        Vertex( 0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f),
        Vertex(-0.5f, 0.5f, 0.5f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f),

        // Back
        Vertex(-0.5f,-0.5f,-0.5f,   0.0f, 0.0f,   0.0f, 0.0f,-1.0f),
        Vertex(-0.5f, 0.5f,-0.5f,   0.0f, 1.0f,   0.0f, 0.0f,-1.0f),
        Vertex( 0.5f,-0.5f,-0.5f,   1.0f, 0.0f,   0.0f, 0.0f,-1.0f),
        Vertex( 0.5f,-0.5f,-0.5f,   1.0f, 0.0f,   0.0f, 0.0f,-1.0f),
        Vertex(-0.5f, 0.5f,-0.5f,   0.0f, 1.0f,   0.0f, 0.0f,-1.0f),
        Vertex( 0.5f, 0.5f,-0.5f,   1.0f, 1.0f,   0.0f, 0.0f,-1.0f),

        // Left
        Vertex(-0.5f,-0.5f, 0.5f,   0.0f, 1.0f,  -1.0f, 0.0f, 0.0f),
        Vertex(-0.5f, 0.5f,-0.5f,   1.0f, 0.0f,  -1.0f, 0.0f, 0.0f),
        Vertex(-0.5f,-0.5f,-0.5f,   0.0f, 0.0f,  -1.0f, 0.0f, 0.0f),
        Vertex(-0.5f,-0.5f, 0.5f,   0.0f, 1.0f,  -1.0f, 0.0f, 0.0f),
        Vertex(-0.5f, 0.5f, 0.5f,   1.0f, 1.0f,  -1.0f, 0.0f, 0.0f),
        Vertex(-0.5f, 0.5f,-0.5f,   1.0f, 0.0f,  -1.0f, 0.0f, 0.0f),

        // Right
        Vertex( 0.5f,-0.5f, 0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f),
        Vertex( 0.5f,-0.5f,-0.5f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f),
        Vertex( 0.5f, 0.5f,-0.5f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f),
        Vertex( 0.5f,-0.5f, 0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f),
        Vertex( 0.5f, 0.5f,-0.5f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f),
        Vertex( 0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f)
    };
    
    mesh.Generate(vertices);
    init = true;
}

void Cube::Render() const
{
    if (!init)
        Init();
    
    mesh.Render();
}


bool Sphere::init = false;
Mesh Sphere::mesh;
const int Sphere::SLICES = 64;
const int Sphere::STACKS = 64;

void Sphere::Init()
{
    std::vector<Vertex> vertices;
    
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

            vertices.push_back(Vertex(Vector3(x, y, z), Vector2(u, v), Vector3(x, y, z)));
        }
    }
    
    std::vector<unsigned> indicies;
    for (int i = 0; i < SLICES * STACKS + SLICES; i++)
    {
        indicies.push_back(i);
        indicies.push_back(i + SLICES + 1);
        indicies.push_back(i + SLICES);
    
        indicies.push_back(i + SLICES + 1);
        indicies.push_back(i);
        indicies.push_back(i + 1);
    }
    
    mesh.Generate(vertices, indicies);
    init = true;
}

void Sphere::Render() const
{
    if (!init)
        Init();
    
    mesh.Render();
}

bool Plane::init = false;
Mesh Plane::mesh;

void Plane::Init()
{
    std::vector<Vertex> vertices
    {
        //       X     Y     Z       U     V           Normal
        Vertex(-0.5f, 0.0f,-0.5f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f),
        Vertex(-0.5f, 0.0f, 0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f),
        Vertex( 0.5f, 0.0f,-0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f),
        Vertex( 0.5f, 0.0f,-0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f),
        Vertex(-0.5f, 0.0f, 0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f),
        Vertex( 0.5f, 0.0f, 0.5f,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f),
    };
    
    mesh.Generate(vertices);
    init = true;
}

void Plane::Render() const
{
    if (!init)
        Init();
    
    mesh.Render();
}

bool Billboard::init = false;
Mesh Billboard::mesh;

void Billboard::Init()
{
    std::vector<Vertex> vertices
    {
        //       X    Y     Z        U     V      Normal
        Vertex( 0.5f, 0.5f, 0.0f,   1.0f, 1.0f,   0, 0, 0),
        Vertex(-0.5f, 0.5f, 0.0f,   0.0f, 1.0f,   0, 0, 0),
        Vertex( 0.5f,-0.5f, 0.0f,   1.0f, 0.0f,   0, 0, 0),
        Vertex( 0.5f,-0.5f, 0.0f,   1.0f, 0.0f,   0, 0, 0),
        Vertex(-0.5f, 0.5f, 0.0f,   0.0f, 1.0f,   0, 0, 0),
        Vertex(-0.5f,-0.5f, 0.0f,   0.0f, 0.0f,   0, 0, 0),
    };
    
    mesh.Generate(vertices);
    init = true;
}

void Billboard::Render() const
{
    if (!init)
        Init();
    
    mesh.Render();
}

