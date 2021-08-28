
#include "AssetManager.h"
#include "FileSystem.h"
#include "Debug.h"

using namespace DrageEngine;

const std::string AssetManager::ASSETS_DIR = "Assets/";

AssetManager::AssetManager()
{
    // Load mapping from file name to path
    std::vector<std::string> files;
    if (GetFilesInDirectory(AssetManager::ASSETS_DIR, files, true))
    {
        for (std::vector<std::string>::iterator i = files.begin(); i != files.end(); i++)
        {
            std::string filename = GetFileName(*i);
            m_pathMap[filename] = *i;
        }
    }
}

Image* AssetManager::GetImage(const std::string &filename)
{
    std::string extension = GetFileExtension(filename);
    if (m_imageFactory.ExtensionSupported(extension))
    {
        Image *image = m_imageFactory.Create(extension);
        if (image->Load(GetAssetPath(filename)))
            return image;
    }
    else
    {
        WARNING("Image file format not supported: " + filename);
    }
    return NULL;
}

Texture* AssetManager::GetTexture(const std::string &filename)
{
    TextureCache::iterator i = m_textureCache.find(filename);
    if (i != m_textureCache.end())
        return i->second;
    
    std::string extension = GetFileExtension(filename);
    if (m_imageFactory.ExtensionSupported(extension))
    {
        Image *image = m_imageFactory.Create(extension);
        if (image->Load(GetAssetPath(filename)))
        {
            Texture *texture = new Texture(image);
            m_textureCache[filename] = texture;
            delete image;
            return texture;
        }
    }
    else
    {
        WARNING("Image file format not supported: " + filename);
    }
    return NULL;
}

Cubemap* AssetManager::GetCubemap(const std::string &filename)
{
    CubemapCache::iterator i = m_cubemapCache.find(filename);
    if (i != m_cubemapCache.end())
        return i->second;
    
    std::string extension = GetFileExtension(filename);
    if (extension == "cubemap")
    {
        Cubemap *cubemap = new Cubemap();
        if (cubemap->Load(GetAssetPath(filename)))
        {
            m_cubemapCache[filename] = cubemap;
            return cubemap;
        }
    }
    else
    {
        WARNING("Cubemap file format not supported: " + filename);
    }
    return NULL;
}

Shader* AssetManager::GetShader(const std::string &filename)
{
    ShaderCache::iterator i = m_shaderCache.find(filename);
    if (i != m_shaderCache.end())
        return i->second;
    
    std::string extension = GetFileExtension(filename);
    if (extension == "shader")
    {
        Shader *shader = new Shader();
        if (shader->Load(GetAssetPath(filename)))
        {
            m_shaderCache[filename] = shader;
            return shader;
        }
    }
    else
    {
        WARNING("Shader file format not supported: " + filename);
    }
    return NULL;
}

Material* AssetManager::GetMaterial(const std::string &filename)
{
    MaterialCache::iterator i = m_materialCache.find(filename);
    if (i != m_materialCache.end())
        return i->second;
    
    std::string extension = GetFileExtension(filename);
    if (extension == "material")
    {
        Material *material = new Material();
        if (material->Load(GetAssetPath(filename)))
        {
            m_materialCache[filename] = material;
            return material;
        }
    }
    else
    {
        WARNING("Material file format not supported: " + filename);
    }
    return NULL;
}

Model* AssetManager::GetModel(const std::string &filename)
{
    ModelCache::iterator i = m_modelCache.find(filename);
    if (i != m_modelCache.end())
        return i->second;
    
    std::string extension = GetFileExtension(filename);
    if (m_modelFactory.ExtensionSupported(extension))
    {
        Model *model = m_modelFactory.Create(extension);
        if (model->Load(GetAssetPath(filename)))
            return model;
    }
    else
    {
        WARNING("Model file format not supported: " + filename);
    }
    return NULL;
}

AudioClip* AssetManager::GetAudio(const std::string &filename)
{
    AudioCache::iterator i = m_audioCache.find(filename);
    if (i != m_audioCache.end())
        return i->second;
    
    std::string extension = GetFileExtension(filename);
    if (m_audioFactory.ExtensionSupported(extension))
    {
        Audio *audio = m_audioFactory.Create(extension);
        if (audio->Load(GetAssetPath(filename)))
        {
            AudioClip *clip = new AudioClip(audio);
            m_audioCache[filename] = clip;
            delete audio;
            return clip;
        }
    }
    else
    {
        WARNING("Audio file format not supported: " + filename);
    }
    return NULL;
}

Component* AssetManager::GetComponent(const std::string &name)
{
    if (m_componentFactory.IsSupported(name))
    {
        Component *component = m_componentFactory.Create(name);
        return component;
    }
    else
    {
        WARNING("Component type not supported: " + name);
    }
    return NULL;
}

bool AssetManager::IsValidComponentType(const std::string &name)
{
    return m_componentFactory.IsSupported(name);
}

std::string AssetManager::GetAssetPath(const std::string &name)
{
    if (m_pathMap[name] != "")
    {
        return m_pathMap[name];
    }
    else
    {
        ERROR("Could not locate asset: " + name);
        return name;
    }
}
