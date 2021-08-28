
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
            pathMap[filename] = *i;
        }
    }
}

Image* AssetManager::GetImage(const std::string &filename)
{
    std::string extension = GetFileExtension(filename);
    if (imageFactory.ExtensionSupported(extension))
    {
        Image *image = imageFactory.Create(extension);
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
    TextureCache::iterator i = textureCache.find(filename);
    if (i != textureCache.end())
        return i->second;
    
    std::string extension = GetFileExtension(filename);
    if (imageFactory.ExtensionSupported(extension))
    {
        Image *image = imageFactory.Create(extension);
        if (image->Load(GetAssetPath(filename)))
        {
            Texture *texture = new Texture(image);
            textureCache[filename] = texture;
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
    CubemapCache::iterator i = cubemapCache.find(filename);
    if (i != cubemapCache.end())
        return i->second;
    
    std::string extension = GetFileExtension(filename);
    if (extension == "cubemap")
    {
        Cubemap *cubemap = new Cubemap();
        if (cubemap->Load(GetAssetPath(filename)))
        {
            cubemapCache[filename] = cubemap;
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
    ShaderCache::iterator i = shaderCache.find(filename);
    if (i != shaderCache.end())
        return i->second;
    
    std::string extension = GetFileExtension(filename);
    if (extension == "shader")
    {
        Shader *shader = new Shader();
        if (shader->Load(GetAssetPath(filename)))
        {
            shaderCache[filename] = shader;
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
    MaterialCache::iterator i = materialCache.find(filename);
    if (i != materialCache.end())
        return i->second;
    
    std::string extension = GetFileExtension(filename);
    if (extension == "material")
    {
        Material *material = new Material();
        if (material->Load(GetAssetPath(filename)))
        {
            materialCache[filename] = material;
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
    ModelCache::iterator i = modelCache.find(filename);
    if (i != modelCache.end())
        return i->second;
    
    std::string extension = GetFileExtension(filename);
    if (modelFactory.ExtensionSupported(extension))
    {
        Model *model = modelFactory.Create(extension);
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
    AudioCache::iterator i = audioCache.find(filename);
    if (i != audioCache.end())
        return i->second;
    
    std::string extension = GetFileExtension(filename);
    if (audioFactory.ExtensionSupported(extension))
    {
        Audio *audio = audioFactory.Create(extension);
        if (audio->Load(GetAssetPath(filename)))
        {
            AudioClip *clip = new AudioClip(audio);
            audioCache[filename] = clip;
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
    if (componentFactory.IsSupported(name))
    {
        Component *component = componentFactory.Create(name);
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
    return componentFactory.IsSupported(name);
}

std::string AssetManager::GetAssetPath(const std::string &name)
{
    if (pathMap[name] != "")
    {
        return pathMap[name];
    }
    else
    {
        ERROR("Could not locate asset: " + name);
        return name;
    }
}
