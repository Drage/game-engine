
#include "Model.h"
#include "Debug.h"
#include "FileSystem.h"

using namespace DrageEngine;

Model::Model()
{
    
}

Model::~Model()
{
    for (std::vector<Mesh*>::iterator i = meshList.begin(); i != meshList.end(); i++)
        delete (*i);
    meshList.clear();
}

bool Model::Load(const std::string &filename)
{
    name = GetFileName(filename);
    ERROR("Cannot load model as generic base type: " + filename);
    return false;
}

int Model::GetMeshCount() const
{
    return (int)meshList.size();
}

Mesh* Model::GetMesh(int index) const
{
    if (index >= 0 && (unsigned)index < meshList.size())
        return meshList[index];
    else
        return NULL;
}

const std::string& Model::GetName() const
{
    return name;
}

const Bounds& Model::GetBounds() const
{
    return bounds;
}
