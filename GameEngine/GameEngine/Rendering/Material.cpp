
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

Material::Material()
{
    
}

bool Material::Load(const std::string &filename)
{
    m_name = GetFileName(filename);
    
    XMLDocument xml;
    if (!xml.Load(filename))
        return false;
    
    ParamList params;
    xml.root.ToParamList(params);
    
    std::string shaderFilename = params.Get<std::string>("shader");
    
    m_shader = app->assets->GetShader(shaderFilename);
    
    params.Merge(m_shader->GetDefaultUniformValues(), false);
    
    Shader::UniformList uniforms = m_shader->GetActiveUniforms();
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
                m_attributes.push_back(attribute);
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
                m_attributes.push_back(attribute);
                break;
            }
        }
    }
    
    return true;
}

Shader* Material::GetShader() const
{
    return m_shader;
}

void Material::ApplyUniforms() const
{
    int textureIndex = 0;
    for (int i = 0; i < m_attributes.size(); i++)
    {
        switch (m_attributes[i].type)
        {
            case Shader::Uniform::Float:
                m_shader->SetUniform(m_attributes[i].location, *(float*)m_attributes[i].value);
                break;
                
            case Shader::Uniform::Int:
                m_shader->SetUniform(m_attributes[i].location, *(int*)m_attributes[i].value);
                break;
                
            case Shader::Uniform::Vec2:
                m_shader->SetUniform(m_attributes[i].location, *(Vector2*)m_attributes[i].value);
                break;
                
            case Shader::Uniform::Vec3:
                m_shader->SetUniform(m_attributes[i].location, *(Vector3*)m_attributes[i].value);
                break;
                
            case Shader::Uniform::Vec4:
                m_shader->SetUniform(m_attributes[i].location, *(Vector4*)m_attributes[i].value);
                break;
                
            case Shader::Uniform::Texture:
                m_shader->SetUniform(m_attributes[i].location, textureIndex);
                glActiveTexture(GL_TEXTURE0 + textureIndex++);
                glBindTexture(GL_TEXTURE_2D, *(int*)m_attributes[i].value);
                break;
                
            case Shader::Uniform::Cubemap:
                m_shader->SetUniform(m_attributes[i].location, textureIndex);
                glActiveTexture(GL_TEXTURE0 + textureIndex++);
                glBindTexture(GL_TEXTURE_CUBE_MAP, *(int*)m_attributes[i].value);
                break;
        }
    }
}
