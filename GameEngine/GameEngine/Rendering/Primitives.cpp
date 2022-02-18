
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

Mesh* Primitive::cube = NULL;

const Mesh* Primitive::Cube()
{
    if (cube != NULL)
        return cube;
    
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
    
    cube = new Mesh();
    cube->Generate(vertices);
    return cube;
}


Mesh* Primitive::sphere = NULL;

const int Primitive::SPHERE_H_SEGMENTS = 64;
const int Primitive::SPHERE_V_SEGMENTS = 64;

const Mesh* Primitive::Sphere()
{
    if (sphere != NULL)
        return sphere;
    
    std::vector<Vertex> vertices;
    
    for (int i = 0; i <= SPHERE_V_SEGMENTS; i++)
    {
        float v = i / (float)SPHERE_V_SEGMENTS;
        float phi = v * PI;

        for (int j = 0; j <= SPHERE_H_SEGMENTS; j++)
        {
            float u = j / (float)SPHERE_H_SEGMENTS;
            float theta = u * PI * 2;

            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            vertices.push_back(Vertex(Vector3(x, y, z), Vector2(u, v), Vector3(x, y, z)));
        }
    }
    
    std::vector<unsigned> indicies;
    for (int i = 0; i < SPHERE_H_SEGMENTS * SPHERE_V_SEGMENTS + SPHERE_H_SEGMENTS; i++)
    {
        indicies.push_back(i);
        indicies.push_back(i + SPHERE_H_SEGMENTS + 1);
        indicies.push_back(i + SPHERE_H_SEGMENTS);
    
        indicies.push_back(i + SPHERE_H_SEGMENTS + 1);
        indicies.push_back(i);
        indicies.push_back(i + 1);
    }
    
    sphere = new Mesh();
    sphere->Generate(vertices, indicies);
    return sphere;
}


Mesh* Primitive::plane = NULL;

const Mesh* Primitive::Plane()
{
    if (plane != NULL)
        return plane;
    
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
    
    plane = new Mesh();
    plane->Generate(vertices);
    return plane;
}


Mesh* Primitive::billboard = NULL;

const Mesh* Primitive::Billboard()
{
    if (billboard != NULL)
        return billboard;
    
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
    
    billboard = new Mesh();
    billboard->Generate(vertices);
    return billboard;
}
