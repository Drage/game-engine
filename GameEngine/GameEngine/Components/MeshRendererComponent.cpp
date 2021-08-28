
#include "MeshRendererComponent.h"
#include "Application.h"

using namespace DrageEngine;

void MeshRendererComponent::Init(const ParamList &params)
{
    std::string modelName = params.Get<std::string>("model");
    m_model = app->assets->GetModel(modelName);
    
    std::string materialName = params.Get<std::string>("material");
    m_material = materialName != "" ? app->assets->GetMaterial(materialName) : NULL;
}

void MeshRendererComponent::Render(Renderer *renderer, const Transform *transform) const
{
    if (m_model)
    {
        for (int i = 0; i < m_model->GetMeshCount(); i++)
        {
            Mesh *mesh = m_model->GetMesh(i);
            
            Material *material = m_material;
            
            if (!material)
                material = mesh->GetMaterial();
            
            if (!material)
                material = app->assets->GetMaterial("Default.material");
            
            renderer->Render(mesh, transform, material);
        }
    }
}
