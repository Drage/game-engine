
#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>
#include <OpenGL/gl.h>
#include <vector>
#include "Terrain.h"
#include "Application.h"
#include "MathUtils.h"

using namespace DrageEngine;

Terrain::Terrain()
{
    maxHeight = 10;
    resolution = 1;
}

Terrain::Terrain(const std::string &heightmap, float maxHeight, float resolution)
{
    Load(heightmap, maxHeight, resolution);
}

Terrain::~Terrain()
{
    heightmap.Deallocate();
}

bool Terrain::Load(const std::string &heightmapImageFile, float maxHeight, float resolution)
{
    Image *img = app->assets->GetImage(heightmapImageFile);
    if (!img)
        return false;
    
    this->maxHeight = maxHeight;
    this->resolution = resolution;
    
    int sizeX = img->GetWidth();
    int sizeZ = img->GetHeight();
    heightmap.Allocate(sizeX, sizeZ);
    
    unsigned char *pixels = img->GetPixels();
    int bpp = img->GetBPP();
    for (int z = 0; z < sizeZ; z++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            unsigned char *pixelValue = &pixels[(z * sizeX + x) * bpp / 8];
            heightmap(x, z) = GetHeightFromPixelData(pixelValue, bpp);
        }
    }
    
    normals.Allocate(sizeX, sizeZ);
    
    InitRenderBuffers();
    return true;
}

float Terrain::GetHeightFromPixelData(unsigned char* data, unsigned bpp) const
{
    switch (bpp)
    {
        case 8:
            return data[0] / 256.0f;
        case 24:
            return ((int)data[0] - (int)data[2]) / 256.0f;
        default:
            ERROR("Heightmap bpp not supported: " + std::to_string(bpp));
            break;
    }
    return 0.0f;
}

float Terrain::HeightAt(const Vector3 &worldPosition) const
{
    return HeightAt(worldPosition.x, worldPosition.z);
}

float Terrain::HeightAt(float x, float z) const
{
    x /= resolution;
    z /= resolution;
    
    const int sizeX = heightmap.GetWidth();
    const int sizeZ = heightmap.GetHeight();
    
    int u0 = (int)floor(x);
    int u1 = u0 + 1;
    int v0 = (int)floor(z);
    int v1 = v0 + 1;
    
    if (u0 < 0 || u1 >= sizeX || v0 < 0 || v1 >= sizeZ)
        return 0;

    float h00 = heightmap(u0, v0);
    float h10 = heightmap(u1, v0);
    float h01 = heightmap(u0, v1);
    float h11 = heightmap(u1, v1);
    
    float percentU = x - u0;
    float percentV = z - v0;
    
    float dU, dV;
    if (percentU > percentV)
    {
        dU = h10 - h00;
        dV = h11 - h10;
    }
    else
    {
        dU = h11 - h01;
        dV = h01 - h00;
    }
    
    return (h00 + (dU * percentU) + (dV * percentV)) * maxHeight;
}

Vector3 Terrain::NormalAt(const Vector3 &worldPosition) const
{
    return NormalAt(worldPosition.x, worldPosition.z);
}

Vector3 Terrain::NormalAt(float x, float z) const
{
    x /= resolution;
    z /= resolution;
    
    const int sizeX = heightmap.GetWidth();
    const int sizeZ = heightmap.GetHeight();
    
    int u0 = (int)floor(x);
    int u1 = u0 + 1;
    int v0 = (int)floor(z);
    int v1 = v0 + 1;
    
    if (u0 < 0 || u1 >= sizeX || v0 < 0 || v1 >= sizeZ)
        return 0;
    
    Vector3 n00 = normals(u0, v0);
    Vector3 n10 = normals(u1, v0);
    Vector3 n01 = normals(u0, v1);
    Vector3 n11 = normals(u1, v1);
    
    float percentU = x - u0;
    float percentV = z - v0;
    
    Vector3 dU, dV;
    if (percentU > percentV)
    {
        dU = n10 - n00;
        dV = n11 - n10;
    }
    else
    {
        dU = n11 - n01;
        dV = n01 - n00;
    }
    
    return n00 + (dU * percentU) + (dV * percentV);
}

void Terrain::InitRenderBuffers()
{
    const int sizeX = heightmap.GetWidth();
    const int sizeZ = heightmap.GetHeight();
    
    const int numVerts = sizeX * sizeZ;
    std::vector<Vertex> vertexBuffer(numVerts);
    std::vector<Vector2> texCoordBuffer(numVerts);
    
    float worldScaleX = (sizeX - 1) * resolution;
    float worldScaleZ = (sizeZ - 1) * resolution;
    
    for (int z = 0; z < sizeZ - 1; z++)
    {
        for (int x = 0; x < sizeX - 1; x++)
        {
            int index = (z * sizeX) + x;
            float height = heightmap(x, z);

            float S = (x / (float)(sizeX - 1));
            float T = (z / (float)(sizeZ - 1));

            float X = S * worldScaleX;
            float Y = height * maxHeight;
            float Z = T * worldScaleZ;
            
            vertexBuffer[index] = Vertex(Vector3(X, Y, Z), Vector2(x, z), Vector3(0));
            texCoordBuffer[index] = Vector2(S, T);
        }
    }
    
    const int numTriangles = (sizeX - 1) * (sizeZ - 1) * 2;
    const int numIndexes = numTriangles * 3;
    std::vector<unsigned> indexBuffer(numIndexes);
    unsigned index = 0;
    for (int z = 0; z < sizeZ - 1; z++)
    {
        for (int x = 0; x < sizeX - 1; x++)
        {
            int vertexIndex = (z * sizeX) + x;
            
            indexBuffer[index++] = vertexIndex;
            indexBuffer[index++] = vertexIndex + sizeX + 1;
            indexBuffer[index++] = vertexIndex + 1;
            
            indexBuffer[index++] = vertexIndex;
            indexBuffer[index++] = vertexIndex + sizeX;
            indexBuffer[index++] = vertexIndex + sizeX + 1;
        }
    }

    for (int i = 0; i < indexBuffer.size(); i += 3)
    {
        Vector3 v0 = vertexBuffer[indexBuffer[i + 0]].position;
        Vector3 v1 = vertexBuffer[indexBuffer[i + 1]].position;
        Vector3 v2 = vertexBuffer[indexBuffer[i + 2]].position;

        Vector3 normal = Vector3::Cross(v1 - v0, v2 - v0).Normalize();

        vertexBuffer[indexBuffer[i + 0]].normal += normal;
        vertexBuffer[indexBuffer[i + 1]].normal += normal;
        vertexBuffer[indexBuffer[i + 2]].normal += normal;
    }
    for (int i = 0; i < vertexBuffer.size(); i++)
    {
        vertexBuffer[i].normal.Normalize();
    }
    
    for (int z = 0; z < sizeZ; z++)
    {
        for (int x = 0; x < sizeX; x++)
            normals(x, z) = vertexBuffer[z * sizeX + x].normal;
    }
    
    mesh.Generate(vertexBuffer, indexBuffer, texCoordBuffer);
}

const Mesh* Terrain::GetMesh() const
{
    return &mesh;
}
