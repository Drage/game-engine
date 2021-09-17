
#include "SpriteComponent.h"
#include "Application.h"
#include "Primitives.h"
#include "Material.h"

using namespace DrageEngine;

void SpriteComponent::Init(const ParamList &params)
{
    Material *baseMaterial = app->assets->GetMaterial("UI.material");
    material = new Material(*baseMaterial);
    
    std::string textureName = params.Get<std::string>("texture");
    Texture *texture = app->assets->GetTexture(textureName);
    
    material->SetAttributeValue("diffuseMap", texture->GetID());
    material->SetAttributeValue("textureWidth", texture->GetWidth());
    material->SetAttributeValue("textureHeight", texture->GetHeight());
    material->SetAttributeValue("origin", params.Get<Vector2>("origin", Vector2(0)));
    material->SetAttributeValue("nativeTextureScale", params.Get<bool>("nativeTextureScale", false));
    
    billboard = new Renderable(Primitive::Billboard(), material, entity, RenderOption::NO_FRUSTUM_CULL);
}

SpriteComponent::~SpriteComponent()
{
    delete billboard;
}

