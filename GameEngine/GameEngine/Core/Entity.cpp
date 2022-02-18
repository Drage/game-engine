
#include <algorithm>
#include "Entity.h"
#include "Debug.h"
#include "Random.h"
#include "Application.h"

using namespace DrageEngine;

Entity::Entity()
{
    parent = NULL;
    destroyed = false;
    started = false;
    prefabRoot = NULL;
}

Entity::~Entity()
{
    for (auto i : components)
        delete i;
    
    for (auto i : children)
        delete i;
}

void Entity::Init(ParamList &params)
{
    id = params.Get<std::string>("id", Random::UUID());
    name = params.Get<std::string>("name");
    active = params.Get<bool>("active", true);
    transform.scale = params.Get<Vector3>("scale", Vector3::ONE);
    transform.rotation = params.Get<Quaternion>("rotation", Quaternion::IDENTITY);
    transform.position = params.Get<Vector3>("position");
}

void Entity::Start()
{
    started = true;
    
    for (auto i : components)
        i->Start();
    
    for (auto i : children)
        i->Start();
}

void Entity::Update()
{
    if (!started && !app->IsInEditMode())
        Start();
        
    for (auto i : components)
    {
        if (i->IsEnabled() && (i->ExecuteInEditMode() || !app->IsInEditMode()))
            i->Update();
    }
    
    for (auto i : children)
        i->Update();
}

void Entity::SetParent(Entity *entity)
{
    parent = entity;
    
    if (std::find(entity->children.begin(), entity->children.end(), this) == entity->children.end())
        entity->children.push_back(this);
}

void Entity::AddChild(Entity *entity)
{
    if (std::find(entity->children.begin(), entity->children.end(), this) == entity->children.end())
        children.push_back(entity);
    
    entity->parent = this;
}

bool Entity::RemoveChild(Entity *entity)
{
    for (auto i = children.begin(); i != children.end(); i++)
    {
        if (*i == entity)
        {
            children.erase(i);
            return true;
        }
    }
    return false;
}

Entity* Entity::GetChild(const std::string &name)
{
    for (auto child : children)
    {
        if (child->GetName() == name)
            return child;
    }
    return NULL;
}

void Entity::GetAllChildrenInHierarchy(std::vector<Entity*> &entities)
{
    for (auto child : children)
    {
        entities.push_back(child);
        child->GetAllChildrenInHierarchy(entities);
    }
}

const std::vector<Entity*>& Entity::GetChildren() const
{
    return children;
}

void Entity::AddComponent(Component *component)
{
    if (component == NULL)
    {
        ERROR("Attempting to add null component to Entity: " + name);
        return;
    }
        
    components.push_back(component);
    component->entity = this;
    component->transform = &transform;
}

bool Entity::RemoveComponent(Component *component)
{
    for (auto i = components.begin(); i != components.end(); i++)
    {
        if (*i == component)
        {
            components.erase(i);
            return true;
        }
    }
    return false;
}

void Entity::SetPrefabRoot(Entity *entity)
{
    prefabRoot = entity;
}

Transform Entity::GetGlobalTransform() const
{
    Transform global = this->transform;
    const Entity *entity = this;
    while (entity->parent != NULL)
    {
        entity = entity->parent;
        global = entity->transform * global;
    }
    return global;
}
