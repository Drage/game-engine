
#include "Scene.h"
#include "Application.h"

using namespace DrageEngine;

Scene::Scene()
{
    
}

Scene::Scene(const std::string filename)
{
    Load(filename);
}

Scene::~Scene()
{
    Clear();
}

void Scene::Clear()
{
    for (GameObjectList::iterator i = m_objects.begin(); i != m_objects.end(); i++)
        delete (*i);
    m_objects.clear();
}

void Scene::Start()
{
    app->SetActiveScene(this);
    
    for (GameObjectList::const_iterator i = m_objects.begin(); i != m_objects.end(); i++)
        (*i)->Start();
}

void Scene::Update()
{
    if (m_objects.size() > 0)
    {
        for (GameObjectList::iterator i = m_objects.begin(); i != (m_objects.end()--); )
        {
            if ((*i)->IsDestroyed())
            {
                delete *i;
                i = m_objects.erase(i);
            }
            else
            {
                if ((*i)->IsActive())
                    (*i)->Update();
                
                i++;
            }
        }
    }
}

void Scene::Render(Renderer *renderer)
{
    for (GameObjectList::const_iterator i = m_objects.begin(); i != m_objects.end(); i++)
    {
        if ((*i)->IsActive())
            (*i)->Render(renderer);
    }
}

void Scene::Add(GameObject *object)
{
    if (object->GetParent() == NULL)
        m_objects.push_back(object);
}

bool Scene::Remove(GameObject *object)
{
    for (GameObjectList::iterator i = m_objects.begin(); i != m_objects.end(); i++)
    {
        if ((*i) == object)
        {
            delete (*i);
            m_objects.erase(i);
            return true;
        }
    }
    return false;
}

bool Scene::Load(const std::string &filename)
{
    Clear();
    
    XMLDocument xml;
    if (!xml.Load(app->assets->GetAssetPath(filename)))
        return false;

    LoadObjects(&xml.root);
    return true;
}

GameObject* Scene::LoadPrefab(const std::string &filename, ParamList *params)
{
    XMLDocument xml;
    if (!xml.Load(app->assets->GetAssetPath(filename)))
        return NULL;

    GameObject *instance = new GameObject();
    
    if (params)
        instance->Init(*params);
    
    LoadObjects(&xml.root, instance);
    
    Add(instance);
    
    return instance;
}

void Scene::LoadObjects(const XMLDocument::Element *xml, GameObject *parent, ParamList *overrideParams)
{
    for (XMLDocument::ElementList::const_iterator i = xml->subElements.begin(); i != xml->subElements.end(); i++)
    {
        std::string tag = (*i)->name;
        if (tag == "GameObject")
        {
            GameObject *gameObject = new GameObject();
            
            if (parent)
                parent->AddChild(gameObject);
            else
                Add(gameObject);
            
            ParamList params;
            (*i)->ToParamList(params);
            if (overrideParams)
                params.Merge(*overrideParams, true);
             
            gameObject->Init(params);
            
            LoadObjects((*i), gameObject);
        }
        else if (tag == "Prefab")
        {
            ParamList params;
            (*i)->ToParamList(params);
            
            std::string filename = params.Get<std::string>("prefab");
            params.Remove("prefab");
            
            GameObject *instance = LoadPrefab(filename, &params);
            LoadObjects((*i), instance);
            
            if (parent)
                parent->AddChild(instance);
        }
        else if (parent != NULL && app->assets->IsValidComponentType(tag))
        {
            Component *component = app->assets->GetComponent(tag);
            
            ParamList params;
            (*i)->ToParamList(params);
            if (overrideParams)
                params.Merge(*overrideParams, true);
            
            bool enabled = params.Get<bool>("enabled", true);
            component->SetEnabled(enabled);
            
            parent->AddComponent(component);
            
            component->Init(params);
        }
    }
}

GameObject* Scene::Find(const std::string &path, GameObject *parent) const
{
    std::string name = path;
    std::string subPath = "";
    
    size_t slashIndex = path.find("/");
    if (slashIndex != std::string::npos)
    {
        name = path.substr(0, slashIndex);
        subPath = path.substr(slashIndex + 1, path.length() - slashIndex - 1);
    }
    
    if (!parent)
    {
        for (GameObjectList::const_iterator i = m_objects.begin(); i != m_objects.end(); i++)
        {
            if ((*i)->GetName() == name)
            {
                if (subPath == "")
                    return (*i);
                else
                    return Find(subPath, *i);
            }
        }
    }
    else
    {
        GameObject *child = parent->GetChild(name);
        if (subPath == "")
            return child;
        else
            return Find(subPath, child);
    }
    
    return NULL;
}
