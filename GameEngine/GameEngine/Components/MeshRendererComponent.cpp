
#include "MeshRendererComponent.h"
#include "Application.h"

using namespace DrageEngine;

void MeshRendererComponent::Init(const ParamList &params)
{
    std::string modelName = params.Get<std::string>("model");
    model = app->assets->GetModel(modelName);
    
    std::string materialName = params.Get<std::string>("material");
    material = materialName != "" ? app->assets->GetMaterial(materialName) : NULL;
}

void MeshRendererComponent::Render(Renderer *renderer, const Transform *transform) const
{
    if (model)
    {
        for (int i = 0; i < model->GetMeshCount(); i++)
        {
            Mesh *mesh = model->GetMesh(i);
            
            Material *material = this->material;
            
            if (!material)
                material = mesh->GetMaterial();
            
            if (!material)
                material = app->assets->GetMaterial("Default.material");
            
            renderer->Render(mesh, transform, material);
        }
    }
}
