
#include <algorithm>
#include "GameObject.h"
#include "Debug.h"

using namespace DrageEngine;

GameObject::GameObject()
{
    m_parent = NULL;
    m_destroyed = false;
    m_started = false;
}

GameObject::~GameObject()
{
    for (ComponentList::iterator i = m_components.begin(); i != m_components.end(); i++)
        delete (*i);
    
    for (GameObjectList::iterator i = m_children.begin(); i != m_children.end(); i++)
        delete (*i);
}

void GameObject::Init(ParamList &params)
{
    m_active = params.Get<bool>("active", true);
    m_name = params.Get<std::string>("name");
    transform.scale = params.Get<Vector3>("scale", Vector3::ONE);
    transform.rotation = params.Get<Quaternion>("rotation", Quaternion::IDENTITY);
    transform.position = params.Get<Vector3>("position");
}

void GameObject::Start()
{
    m_started = true;
    
    for (ComponentList::iterator i = m_components.begin(); i != m_components.end(); i++)
        (*i)->Start();
    
    for (GameObjectList::iterator i = m_children.begin(); i != m_children.end(); i++)
        (*i)->Start();
}

void GameObject::Update()
{
    if (!m_started)
        Start();
        
    for (ComponentList::iterator i = m_components.begin(); i != m_components.end(); i++)
    {
        if ((*i)->IsEnabled())
            (*i)->Update();
    }
    
    for (GameObjectList::iterator i = m_children.begin(); i != m_children.end(); i++)
        (*i)->Update();
}

void GameObject::Render(Renderer *renderer, const Transform *transform) const
{
    Transform combinedTransform;
    if (!transform)
        combinedTransform = this->transform;
    else
        combinedTransform = (*transform) * this->transform;
    
    for (ComponentList::const_iterator i = m_components.begin(); i != m_components.end(); i++)
    {
        if ((*i)->IsEnabled())
            (*i)->Render(renderer, &combinedTransform);
    }

    for (GameObjectList::const_iterator i = m_children.begin(); i != m_children.end(); i++)
        (*i)->Render(renderer, &combinedTransform);
}

void GameObject::SetParent(GameObject *obj)
{
    m_parent = obj;
    
    if (std::find(obj->m_children.begin(), obj->m_children.end(), this) == obj->m_children.end())
        obj->m_children.push_back(this);
}

void GameObject::AddChild(GameObject *obj)
{
    if (std::find(obj->m_children.begin(), obj->m_children.end(), this) == obj->m_children.end())
        m_children.push_back(obj);
    
    obj->m_parent = this;
}

bool GameObject::RemoveChild(GameObject *obj)
{
    for (GameObjectList::iterator i = m_children.begin(); i != m_children.end(); i++)
    {
        if (*i == obj)
        {
            m_children.erase(i);
            return true;
        }
    }
    return false;
}

GameObject* GameObject::GetChild(const std::string &name)
{
    for (GameObjectList::iterator i = m_children.begin(); i != m_children.end(); i++)
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
        ERROR("Attempting to add null component to GameObject: " + m_name);
        return;
    }
        
    m_components.push_back(component);
    component->gameObject = this;
    component->transform = &transform;
}

bool GameObject::RemoveComponent(Component *component)
{
    for (ComponentList::iterator i = m_components.begin(); i != m_components.end(); i++)
    {
        if (*i == component)
        {
            m_components.erase(i);
            return true;
        }
    }
    return false;
}
