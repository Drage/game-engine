
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include "Material.h"
#include "FileSystem.h"
#include "XMLDocument.h"
#include "AssetManager.h"
#include "Vector3.h"
#include "Debug.h"
#include "Application.h"

using namespace DrageEngine;

unsigned Material::nextMaterialId = 1;

Material::Material()
{
    id = nextMaterialId++;
}

bool Material::Load(const std::string &filename)
{
    name = GetFileName(filename);
    
    XMLDocument xml;
    if (!xml.Load(filename))
        return false;
    
    ParamList params;
    xml.root.ToParamList(params);
    
    transparent = params.Get<bool>("transparent", false);
    
    std::string shaderFilename = params.Get<std::string>("shader");
    shader = app->assets->GetShader(shaderFilename);
    
    params.Merge(shader->GetDefaultUniformValues(), false);
    
    Shader::UniformList uniforms = shader->GetActiveUniforms();
    for (int i = 0; i < uniforms.size(); i++)
    {
        Shader::Uniform u = uniforms[i];
        switch (u.type)
        {
            case Shader::Uniform::Float:
                AddAttribute<float>(u.location, u.type, params.Get<float>(u.name));
                break;
                
            case Shader::Uniform::Int:
                AddAttribute<int>(u.location, u.type, params.Get<int>(u.name));
                break;
                
            case Shader::Uniform::Vec2:
                AddAttribute<Vector2>(u.location, u.type, params.Get<Vector2>(u.name));
                break;
                
            case Shader::Uniform::Vec3:
                AddAttribute<Vector3>(u.location, u.type, params.Get<Vector3>(u.name));
                break;
                
            case Shader::Uniform::Vec4:
                AddAttribute<Vector4>(u.location, u.type, params.Get<Vector4>(u.name));
                break;
                
            case Shader::Uniform::Texture:
            {
                Attribute attribute;
                attribute.type = u.type;
                attribute.location = u.location;
                std::string filename = params.Get<std::string>(u.name);
                Texture *texture = app->assets->GetTexture(filename);
                attribute.value = malloc(sizeof(int));
                *(int*)(attribute.value) = texture != NULL ? texture->GetID() : 0;
                attributes.push_back(attribute);
                break;
            }
                
            case Shader::Uniform::Cubemap:
            {
                Attribute attribute;
                attribute.type = u.type;
                attribute.location = u.location;
                std::string filename = params.Get<std::string>(u.name);
                Cubemap *cubemap = app->assets->GetCubemap(filename);
                attribute.value = malloc(sizeof(int));
                *(int*)(attribute.value) = cubemap != NULL ? cubemap->GetID() : 0;
                attributes.push_back(attribute);
                break;
            }
        }
    }
    
    return true;
}

const Shader* Material::GetShader() const
{
    return shader;
}

void Material::ApplyUniforms() const
{
    int textureIndex = 0;
    for (int i = 0; i < attributes.size(); i++)
    {
        switch (attributes[i].type)
        {
            case Shader::Uniform::Float:
                shader->SetUniform(attributes[i].location, *(float*)attributes[i].value);
                break;
                
            case Shader::Uniform::Int:
                shader->SetUniform(attributes[i].location, *(int*)attributes[i].value);
                break;
                
            case Shader::Uniform::Vec2:
                shader->SetUniform(attributes[i].location, *(Vector2*)attributes[i].value);
                break;
                
            case Shader::Uniform::Vec3:
                shader->SetUniform(attributes[i].location, *(Vector3*)attributes[i].value);
                break;
                
            case Shader::Uniform::Vec4:
                shader->SetUniform(attributes[i].location, *(Vector4*)attributes[i].value);
                break;
                
            case Shader::Uniform::Texture:
                shader->SetUniform(attributes[i].location, textureIndex);
                glActiveTexture(GL_TEXTURE0 + textureIndex++);
                glBindTexture(GL_TEXTURE_2D, *(int*)attributes[i].value);
                break;
                
            case Shader::Uniform::Cubemap:
                shader->SetUniform(attributes[i].location, textureIndex);
                glActiveTexture(GL_TEXTURE0 + textureIndex++);
                glBindTexture(GL_TEXTURE_CUBE_MAP, *(int*)attributes[i].value);
                break;
        }
    }
}

unsigned Material::GetID() const
{
    return id;
}

bool Material::IsTransparent() const
{
    return transparent;
}
