
#include "ModelOBJ.h"
#include "Debug.h"
#include "FileSystem.h"
#include "AssetManager.h"
#include "Application.h"

using namespace DrageEngine;

ModelOBJ::ModelOBJ()
{

}

ModelOBJ::ModelOBJ(const std::string &filename)
{
    Load(filename);
}

bool ModelOBJ::Load(const std::string &filename)
{
    m_name = GetFileName(filename);
    
    std::ifstream file(filename);
    if (!file.is_open())
    {
        ERROR("Could not open file: " + filename);
        return false;
    }
    
    std::string input;
    char buffer[256] = {0};
    
    std::vector<float> vertexCoords;
    std::vector<float> textureCoords;
    std::vector<float> normals;
    std::vector<int> indexes;
    
    int v[3];
    int vt[3];
    int vn[3];
    char slash;
    
    Mesh *currentMesh = NULL;
    std::vector<Vertex> vertices;
    
    AssetManager *assets = app->assets;
    Material *currentMaterial = NULL;
    
    while (file.good())
    {
        file >> input;
        
        switch (input.at(0))
        {
            case '#': // Comment
            {
                file.getline(buffer, sizeof(buffer));
                break;
            }
                
            case 'g': // New mesh
            case 'o':
            {
                if (currentMesh)
                {
                    currentMesh->Generate(vertices);
                    currentMesh->SetMaterial(currentMaterial);
                    m_meshList.push_back(currentMesh);
                }
                
                std::string name;
                file >> name;
                currentMesh = new Mesh(name);
                vertices.clear();
                break;
            }
                
            case 'v': // Verts
            {
                if (input.length() == 1)
                    ParseVertexCoord(file, vertexCoords);
                else if (input.at(1) == 'n')
                    ParseNormal(file, normals);
                else if (input.at(1) == 't')
                    ParseTextureCoord(file, textureCoords);
                break;
            }
                
            case 'f': // Faces
            {
                file >> v[0] >> slash >> vt[0] >> slash >> vn[0]
                >> v[1] >> slash >> vt[1] >> slash >> vn[1]
                >> v[2] >> slash >> vt[2] >> slash >> vn[2];
                
                // Offset since .obj index's start at 1
                for (int i = 0; i < 3; i++)
                {
                    v[i] -= 1;
                    vt[i] -= 1;
                    vn[i] -= 1;
                }
                
                // Add the triangle to the index buffer
                for (int i = 0; i < 3; i++)
                {
                    Vertex vertex;
                    vertex.position[0] = vertexCoords[v[i] * 3];
                    vertex.position[1] = vertexCoords[v[i] * 3 + 1];
                    vertex.position[2] = vertexCoords[v[i] * 3 + 2];
                    vertex.normal[0] = normals[vn[i] * 3];
                    vertex.normal[1] = normals[vn[i] * 3 + 1];
                    vertex.normal[2] = normals[vn[i] * 3 + 2];
                    vertex.texCoord[0] = textureCoords[vt[i] * 2];
                    vertex.texCoord[1] = textureCoords[vt[i] * 2 + 1];
                    vertices.push_back(vertex);
                    
                    if (vertex.position.x < m_bounds.min.x) m_bounds.min.x = vertex.position.x;
                    if (vertex.position.x > m_bounds.max.x) m_bounds.max.x = vertex.position.x;
                    if (vertex.position.y < m_bounds.min.y) m_bounds.min.y = vertex.position.y;
                    if (vertex.position.y > m_bounds.max.y) m_bounds.max.y = vertex.position.y;
                    if (vertex.position.z < m_bounds.min.z) m_bounds.min.z = vertex.position.z;
                    if (vertex.position.z > m_bounds.max.z) m_bounds.max.z = vertex.position.z;
                }
                break;
            }
                
            case 'u': // Material (usemtl)
            {
                std::string name;
                file >> name;
                currentMaterial = assets->GetMaterial(name);
                break;
            }
                
            default: // Ignore
            {
                file.getline(buffer, sizeof(buffer));
                break;
            }
        }
    }
    
    if (currentMesh)
    {
        currentMesh->Generate(vertices);
        currentMesh->SetMaterial(currentMaterial);
        m_meshList.push_back(currentMesh);
    }
    
    file.close();
    return true;
}

void ModelOBJ::ParseVertexCoord(std::ifstream &file, std::vector<float> &vertexCoords)
{
    float number;
    for (int i = 0; i < 3; i++)
    {
        file >> number;
        vertexCoords.push_back(number);
    }
}

void ModelOBJ::ParseTextureCoord(std::ifstream &file, std::vector<float> &textureCoords)
{
    float number;
    for (int i = 0; i < 2; i++)
    {
        file >> number;
        textureCoords.push_back(number);
    }
}

void ModelOBJ::ParseNormal(std::ifstream &file, std::vector<float> &normals)
{
    float number;
    for (int i = 0; i < 3; i++)
    {
        file >> number;
        normals.push_back(number);
    }
}
