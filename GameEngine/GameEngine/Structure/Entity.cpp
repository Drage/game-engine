
#include <algorithm>
#include "Entity.h"
#include "Debug.h"
#include "Random.h"

using namespace DrageEngine;

Entity::Entity()
{
    parent = NULL;
    destroyed = false;
    started = false;
}

Entity::~Entity()
{
    for (ComponentList::iterator i = components.begin(); i != components.end(); i++)
        delete (*i);
    
    for (EntityList::iterator i = children.begin(); i != children.end(); i++)
        delete (*i);
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
    
    for (ComponentList::iterator i = components.begin(); i != components.end(); i++)
        (*i)->Start();
    
    for (EntityList::iterator i = children.begin(); i != children.end(); i++)
        (*i)->Start();
}

void Entity::Update()
{
    if (!started)
        Start();
        
    for (ComponentList::iterator i = components.begin(); i != components.end(); i++)
    {
        if ((*i)->IsEnabled())
            (*i)->Update();
    }
    
    for (EntityList::iterator i = children.begin(); i != children.end(); i++)
        (*i)->Update();
}

void Entity::SetParent(Entity *obj)
{
    parent = obj;
    
    if (std::find(obj->children.begin(), obj->children.end(), this) == obj->children.end())
        obj->children.push_back(this);
}

void Entity::AddChild(Entity *obj)
{
    if (std::find(obj->children.begin(), obj->children.end(), this) == obj->children.end())
        children.push_back(obj);
    
    obj->parent = this;
}

bool Entity::RemoveChild(Entity *obj)
{
    for (EntityList::iterator i = children.begin(); i != children.end(); i++)
    {
        if (*i == obj)
        {
            children.erase(i);
            return true;
        }
    }
    return false;
}

Entity* Entity::GetChild(const std::string &name)
{
    for (EntityList::iterator i = children.begin(); i != children.end(); i++)
    {
        if ((*i)->GetName() == name)
            return *i;
    }
    return NULL;
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
    for (ComponentList::iterator i = components.begin(); i != components.end(); i++)
    {
        if (*i == component)
        {
            components.erase(i);
            return true;
        }
    }
    return false;
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
