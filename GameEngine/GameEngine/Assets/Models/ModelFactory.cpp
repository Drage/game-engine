
#include <algorithm>
#include "ModelFactory.h"
#include "ModelOBJ.h"

using namespace DrageEngine;

ModelFactory::ModelFactory()
{
    // Register model types with factory
    m_factory.Register<ModelOBJ>("obj");
}

Model* ModelFactory::Create(const std::string &extension)
{
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    return m_factory.Create(ext);
}

bool ModelFactory::ExtensionSupported(const std::string &extension) const
{
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    return m_factory.IsSupported(ext);
}
