
#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <map>
#include "ImageFactory.h"
#include "ModelFactory.h"
#include "AudioFactory.h"
#include "ComponentFactory.h"
#include "Image.h"
#include "Texture.h"
#include "Cubemap.h"
#include "Shader.h"
#include "Material.h"
#include "Model.h"
#include "AudioClip.h"
#include "Component.h"

namespace DrageEngine
{
    class AssetManager
    {
        typedef std::map<std::string, std::string> PathMap;
        typedef std::map<std::string, Texture*> TextureCache;
        typedef std::map<std::string, Cubemap*> CubemapCache;
        typedef std::map<std::string, Shader*> ShaderCache;
        typedef std::map<std::string, Material*> MaterialCache;
        typedef std::map<std::string, Model*> ModelCache;
        typedef std::map<std::string, AudioClip*> AudioCache;
        
        public:
            static const std::string ASSETS_DIR;
            std::string GetAssetPath(const std::string &name);
        
            Image* GetImage(const std::string &filename);
            Texture* GetTexture(const std::string &filename);
            Cubemap* GetCubemap(const std::string &filename);
            Shader* GetShader(const std::string &filename);
            Material* GetMaterial(const std::string &filename);
            Model* GetModel(const std::string &filename);
            AudioClip* GetAudio(const std::string &filename);
            Component* GetComponent(const std::string &name);
        
            bool IsValidComponentType(const std::string &name);
            
            template<typename T>
            void RegisterComponent(const std::string &name)
            {
                componentFactory.Register<T>(name);
            }
        
        
        private:
            friend class Application;
            AssetManager();
        
            PathMap pathMap;
        
            ImageFactory imageFactory;
            ModelFactory modelFactory;
            AudioFactory audioFactory;
            ComponentFactory componentFactory;
        
            TextureCache textureCache;
            CubemapCache cubemapCache;
            ShaderCache shaderCache;
            MaterialCache materialCache;
            ModelCache modelCache;
            AudioCache audioCache;
    };
}

#endif
