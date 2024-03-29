
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
    for (auto i : entities)
        delete i;
    entities.clear();
}

void Scene::Start()
{
    for (auto i : entities)
        i->Start();
}

void Scene::Update()
{
    for (auto i = entities.begin(); i != entities.end(); )
    {
        if ((*i)->IsDestroyed())
        {
            delete *i;
            i = entities.erase(i);
        }
        else
        {
            if ((*i)->IsActive())
                (*i)->Update();
            
            i++;
        }
    }
}

void Scene::Add(Entity *entity)
{
    if (entity->GetParent() == NULL)
        entities.push_back(entity);
}

bool Scene::Remove(Entity *entity)
{
    for (EntityList::iterator i = entities.begin(); i != entities.end(); i++)
    {
        if (*i == entity)
        {
            entities.erase(i);
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

    LoadEntities(&xml.root);
    
    app->SetActiveScene(this);
    return true;
}

Entity* Scene::LoadPrefab(const std::string &filename, ParamList *params)
{
    XMLDocument xml;
    if (!xml.Load(app->assets->GetAssetPath(filename)))
        return NULL;

    Entity *instance = new Entity();
    
    if (params)
        instance->Init(*params);
    
    LoadEntities(&xml.root, instance, instance);
    
    Add(instance);
    
    return instance;
}

void Scene::LoadEntities(const XMLDocument::Element *xml, Entity *parent, Entity *prefabRoot, ParamList *overrideParams)
{
    for (auto i = xml->subElements.begin(); i != xml->subElements.end(); i++)
    {
        std::string tag = (*i)->name;
        if (tag == "Entity")
        {
            Entity *entity = new Entity();
            
            if (parent)
                parent->AddChild(entity);
            else
                Add(entity);

            if (prefabRoot)
                entity->SetPrefabRoot(prefabRoot);
            
            ParamList params;
            (*i)->ToParamList(params);
            if (overrideParams)
                params.Merge(*overrideParams, true);
             
            entity->Init(params);
            
            LoadEntities((*i), entity, prefabRoot);
        }
        else if (tag == "Prefab")
        {
            ParamList params;
            (*i)->ToParamList(params);
            
            std::string filename = params.Get<std::string>("prefab");
            params.Remove("prefab");
            
            Entity *instance = LoadPrefab(filename, &params);
            LoadEntities((*i), instance, instance);
            
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

Entity* Scene::Find(const std::string &path, Entity *parent) const
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
        for (auto i : entities)
        {
            if (i->GetName() == name)
            {
                if (subPath == "")
                    return i;
                else
                    return Find(subPath, i);
            }
        }
    }
    else
    {
        Entity *child = parent->GetChild(name);
        if (subPath == "")
            return child;
        else
            return Find(subPath, child);
    }
    
    return NULL;
}

const Scene::EntityList& Scene::GetEntities() const
{
    return entities;
}
