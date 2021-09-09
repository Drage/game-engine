
#include "Renderable.h"
#include "Application.h"

using namespace DrageEngine;

Renderable::Renderable(const Mesh *mesh, const Material *material, Entity *entity, unsigned options)
{
    this->mesh = mesh;
    this->material = material;
    this->entity = entity;
    this->options = options;
    this->depth = 0;
    
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

void Renderable::SetDepth(float depth)
{
    this->depth = depth;
}

float Renderable::GetDepth() const
{
    return depth;
}

unsigned long Renderable::GetSortKey() const
{
    unsigned long opaque = !material->IsTransparent();
    unsigned long shaderId = material->GetShader()->GetID();
    unsigned long materialId = material->GetID();
    unsigned long meshId = mesh->GetID();
    
    unsigned long depth = (unsigned)this->depth;
    unsigned long renderPriority = material->GetRenderPriority();
    unsigned long depthOrder = renderPriority == 0 ? depth : renderPriority;
    
    // 1 bit for transparent/opaque, 8 bits for shader, 11 bits for material, 12 bits for mesh
    // Max 256 shaders, 2048 materials, and 4096 meshes
    return opaque << 63 | depthOrder << 31 | shaderId << 23 | materialId << 12 | meshId;
}
