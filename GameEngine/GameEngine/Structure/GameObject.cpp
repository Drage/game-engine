
#include <algorithm>
#include "GameObject.h"
#include "Debug.h"

using namespace DrageEngine;

GameObject::GameObject()
{
    parent = NULL;
    destroyed = false;
    started = false;
}

GameObject::~GameObject()
{
    for (ComponentList::iterator i = components.begin(); i != components.end(); i++)
        delete (*i);
    
    for (GameObjectList::iterator i = children.begin(); i != children.end(); i++)
        delete (*i);
}

void GameObject::Init(ParamList &params)
{
    active = params.Get<bool>("active", true);
    name = params.Get<std::string>("name");
    transform.scale = params.Get<Vector3>("scale", Vector3::ONE);
    transform.rotation = params.Get<Quaternion>("rotation", Quaternion::IDENTITY);
    transform.position = params.Get<Vector3>("position");
}

void GameObject::Start()
{
    started = true;
    
    for (ComponentList::iterator i = components.begin(); i != components.end(); i++)
        (*i)->Start();
    
    for (GameObjectList::iterator i = children.begin(); i != children.end(); i++)
        (*i)->Start();
}

void GameObject::Update()
{
    if (!started)
        Start();
        
    for (ComponentList::iterator i = components.begin(); i != components.end(); i++)
    {
        if ((*i)->IsEnabled())
            (*i)->Update();
    }
    
    for (GameObjectList::iterator i = children.begin(); i != children.end(); i++)
        (*i)->Update();
}

void GameObject::Render(Renderer *renderer, const Transform *transform) const
{
    Transform combinedTransform;
    if (!transform)
        combinedTransform = this->transform;
    else
        combinedTransform = (*transform) * this->transform;
    
    for (ComponentList::const_iterator i = components.begin(); i != components.end(); i++)
    {
        if ((*i)->IsEnabled())
            (*i)->Render(renderer, &combinedTransform);
    }

    for (GameObjectList::const_iterator i = children.begin(); i != children.end(); i++)
        (*i)->Render(renderer, &combinedTransform);
}

void GameObject::SetParent(GameObject *obj)
{
    parent = obj;
    
    if (std::find(obj->children.begin(), obj->children.end(), this) == obj->children.end())
        obj->children.push_back(this);
}

void GameObject::AddChild(GameObject *obj)
{
    if (std::find(obj->children.begin(), obj->children.end(), this) == obj->children.end())
        children.push_back(obj);
    
    obj->parent = this;
}

bool GameObject::RemoveChild(GameObject *obj)
{
    for (GameObjectList::iterator i = children.begin(); i != children.end(); i++)
    {
        if (*i == obj)
        {
            children.erase(i);
            return true;
        }
    }
    return false;
}

GameObject* GameObject::GetChild(const std::string &name)
{
    for (GameObjectList::iterator i = children.begin(); i != children.end(); i++)
    {
        if ((*i)->GetName() == name)
            return *i;
    }
    return NULL;
}

void GameObject::AddComponent(Component *component)
{
    if (component == NULL)
    {
        ERROR("Attempting to add null component to GameObject: " + name);
        return;
    }
        
    components.push_back(component);
    component->gameObject = this;
    component->transform = &transform;
}

bool GameObject::RemoveComponent(Component *component)
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
