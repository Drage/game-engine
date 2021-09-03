
#include "Renderable.h"
#include "Application.h"

using namespace DrageEngine;

Renderable::Renderable(const Mesh *mesh, const Material *material, Entity *entity, unsigned options)
{
    this->mesh = mesh;
    this->material = material;
    this->entity = entity;
    this->options = options;
    
    unsigned opaque = !material->IsTransparent();
    unsigned shaderId = material->GetShader()->GetID();
    unsigned materialId = material->GetID();
    unsigned meshId = mesh->GetID();
    
    // 1 bit for transparent/opaque, 8 bits for shader, 11 bits for material, 12 bits for mesh
    // Max 256 shaders, 2048 materials, and 4096 meshes
    sortKey = opaque << 31 | shaderId << 23 | materialId << 12 | meshId;
    
    app->renderer->Register(this);
}

Renderable::~Renderable()
{
    app->renderer->Unregister(this);
}

const Mesh* Renderable::GetMesh() const
{
    return mesh;
}

const Material* Renderable::GetMaterial() const
{
    return material;
}

Entity* Renderable::GetEntity() const
{
    return entity;
}

unsigned Renderable::GetOptions() const
{
    return options;
}

unsigned Renderable::GetSortKey() const
{
    return sortKey;
}
