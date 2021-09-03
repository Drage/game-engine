
#include "MeshRendererComponent.h"
#include "Application.h"

using namespace DrageEngine;

void MeshRendererComponent::Init(const ParamList &params)
{
    std::string modelName = params.Get<std::string>("model");
    model = app->assets->GetModel(modelName);
    
    std::string materialName = params.Get<std::string>("material");
    material = materialName != "" ? app->assets->GetMaterial(materialName) : NULL;
    
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
            
            renderables.push_back(new Renderable(mesh, material, entity));
        }
    }
}

MeshRendererComponent::~MeshRendererComponent()
{
    for (int i = 0; i < renderables.size(); i++)
        delete renderables[i];
}
