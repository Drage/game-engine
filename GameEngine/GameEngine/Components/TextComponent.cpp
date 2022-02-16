
#include "TextComponent.h"
#include "Application.h"
#include "Primitives.h"
#include "Material.h"

using namespace DrageEngine;

void TextComponent::Init(const ParamList &params)
{
//    Material *baseMaterial = app->assets->GetMaterial("UI.material");
//    material = new Material(*baseMaterial);
//
//    std::string fontName = params.Get<std::string>("font", "Roboto.ttf");
//    int fontSize = params.Get<int>("size", 25);
//    Font *font = app->assets->GetFont(fontName, fontSize);
//
//    std::string text = params.Get<std::string>("text");
//    Color color = params.Get<Color>("color", Color::BLACK);
//    Texture *texture = font->Render(text, color);
//
//    material->SetAttributeValue("diffuseMap", texture->GetID());
//    material->SetAttributeValue("textureWidth", texture->GetWidth());
//    material->SetAttributeValue("textureHeight", texture->GetHeight());
//    material->SetAttributeValue("origin", params.Get<Vector2>("origin", Vector2(-1, 1)));
//    material->SetAttributeValue("nativeTextureScale", true);
//
//    billboard = new Renderable(Primitive::Billboard(), material, entity, RenderOption::NO_FRUSTUM_CULL);
}

TextComponent::~TextComponent()
{
//    delete billboard;
}

