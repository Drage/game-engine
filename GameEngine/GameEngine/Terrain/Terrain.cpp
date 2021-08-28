
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include <vector>
#include "Terrain.h"
#include "Application.h"
#include "MathUtils.h"

using namespace DrageEngine;

Terrain::Terrain()
{
    m_maxHeight = 10;
    m_resolution = 1;
}

Terrain::Terrain(const std::string &heightmap, float maxHeight, float resolution)
{
    Load(heightmap, maxHeight, resolution);
}

Terrain::~Terrain()
{
    m_heightmap.Deallocate();
}

bool Terrain::Load(const std::string &heightmap, float maxHeight, float resolution)
{
    Image *img = app->assets->GetImage(heightmap);
    if (!img)
        return false;
    
    m_maxHeight = maxHeight;
    m_resolution = resolution;
    
    int sizeX = img->GetWidth();
    int sizeZ = img->GetHeight();
    m_heightmap.Allocate(sizeX, sizeZ);
    
    unsigned char *pixels = img->GetPixels();
    int bpp = img->GetBPP();
    for (int z = 0; z < sizeZ; z++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            unsigned char *pixelValue = &pixels[(z * sizeX + x) * bpp / 8];
            m_heightmap(x, z) = GetHeightFromPixelData(pixelValue, bpp);
        }
    }
    
    m_normals.Allocate(sizeX, sizeZ);
    
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
    x /= m_resolution;
    z /= m_resolution;
    
    const int sizeX = m_heightmap.GetWidth();
    const int sizeZ = m_heightmap.GetHeight();
    
    int u0 = (int)floor(x);
    int u1 = u0 + 1;
    int v0 = (int)floor(z);
    int v1 = v0 + 1;
    
    if (u0 < 0 || u1 >= sizeX || v0 < 0 || v1 >= sizeZ)
        return 0;

    float h00 = m_heightmap(u0, v0);
    float h10 = m_heightmap(u1, v0);
    float h01 = m_heightmap(u0, v1);
    float h11 = m_heightmap(u1, v1);
    
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
    
    return (h00 + (dU * percentU) + (dV * percentV)) * m_maxHeight;
}

Vector3 Terrain::NormalAt(const Vector3 &worldPosition) const
{
    return NormalAt(worldPosition.x, worldPosition.z);
}

Vector3 Terrain::NormalAt(float x, float z) const
{
    x /= m_resolution;
    z /= m_resolution;
    
    const int sizeX = m_heightmap.GetWidth();
    const int sizeZ = m_heightmap.GetHeight();
    
    int u0 = (int)floor(x);
    int u1 = u0 + 1;
    int v0 = (int)floor(z);
    int v1 = v0 + 1;
    
    if (u0 < 0 || u1 >= sizeX || v0 < 0 || v1 >= sizeZ)
        return 0;
    
    Vector3 n00 = m_normals(u0, v0);
    Vector3 n10 = m_normals(u1, v0);
    Vector3 n01 = m_normals(u0, v1);
    Vector3 n11 = m_normals(u1, v1);
    
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
    const int sizeX = m_heightmap.GetWidth();
    const int sizeZ = m_heightmap.GetHeight();
    
    const int numVerts = sizeX * sizeZ;
    std::vector<Vector3> positionBuffer(numVerts);
    std::vector<Vector3> normalBuffer(numVerts);
    std::vector<Vector2> texBuffer(numVerts);
    std::vector<Vector2> texBuffer2(numVerts);
    
    float worldScaleX = (sizeX - 1) * m_resolution;
    float worldScaleZ = (sizeZ - 1) * m_resolution;
    
    for (int z = 0; z < sizeZ - 1; z++)
    {
        for (int x = 0; x < sizeX - 1; x++)
        {
            int index = (z * sizeX) + x;
            float height = m_heightmap(x, z);

            float S = (x / (float)(sizeX - 1));
            float T = (z / (float)(sizeZ - 1));

            float X = S * worldScaleX;
            float Y = height * m_maxHeight;
            float Z = T * worldScaleZ;

            positionBuffer[index] = Vector3(X, Y, Z);
            normalBuffer[index] = Vector3(0);
            
            texBuffer[index] = Vector2(x, z);
            texBuffer2[index] = Vector2(S, T);
        }
    }
    
    const int numTriangles = (sizeX - 1) * (sizeZ - 1) * 2;
    m_numIndexes = numTriangles * 3;
    std::vector<unsigned> indexBuffer(m_numIndexes);
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
        Vector3 v0 = positionBuffer[indexBuffer[i + 0]];
        Vector3 v1 = positionBuffer[indexBuffer[i + 1]];
        Vector3 v2 = positionBuffer[indexBuffer[i + 2]];

        Vector3 normal = Vector3::Cross(v1 - v0, v2 - v0).Normalize();

        normalBuffer[indexBuffer[i + 0]] += normal;
        normalBuffer[indexBuffer[i + 1]] += normal;
        normalBuffer[indexBuffer[i + 2]] += normal;
    }
    for (int i = 0; i < normalBuffer.size(); i++)
    {
        normalBuffer[i].Normalize();
    }
    
    for (int z = 0; z < sizeZ; z++)
    {
        for (int x = 0; x < sizeX; x++)
            m_normals(x, z) = normalBuffer[z * sizeX + x];
    }
    
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    
    GLuint vbos[5];
    glGenBuffers(5, vbos);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, positionBuffer.size() * sizeof(Vector3), positionBuffer.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, texBuffer.size() * sizeof(Vector2), texBuffer.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, normalBuffer.size() * sizeof(Vector3), normalBuffer.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[3]);
    glBufferData(GL_ARRAY_BUFFER, texBuffer2.size() * sizeof(Vector2), texBuffer2.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(unsigned), indexBuffer.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Terrain::Render() const
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_numIndexes, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}
