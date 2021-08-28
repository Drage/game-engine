
#include "Model.h"
#include "Debug.h"
#include "FileSystem.h"

using namespace DrageEngine;

Model::Model()
{
    
}

Model::~Model()
{
    for (std::vector<Mesh*>::iterator i = m_meshList.begin(); i != m_meshList.end(); i++)
        delete (*i);
    m_meshList.clear();
}

bool Model::Load(const std::string &filename)
{
    m_name = GetFileName(filename);
    ERROR("Cannot load model as generic base type: " + filename);
    return false;
}

int Model::GetMeshCount() const
{
    return (int)m_meshList.size();
}

Mesh* Model::GetMesh(int index) const
{
    if (index >= 0 && (unsigned)index < m_meshList.size())
        return m_meshList[index];
    else
        return NULL;
}

const std::string& Model::GetName() const
{
    return m_name;
}

const Bounds& Model::GetBounds() const
{
    return m_bounds;
}
