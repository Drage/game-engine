
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include "Cubemap.h"
#include "Application.h"
#include "Debug.h"
#include "FileSystem.h"
#include "XMLDocument.h"

using namespace DrageEngine;

Cubemap::Cubemap()
{
    m_id = 0;
}

Cubemap::~Cubemap()
{
    glDeleteTextures(1, &m_id);
}

bool Cubemap::Load(const std::string &filename)
{
    m_name = GetFileName(filename);
    
    XMLDocument xml;
    if (!xml.Load(filename))
        return false;
    
    ParamList params;
    xml.root.ToParamList(params, true);
    
    std::vector<std::string> faces = {
        params.Get<std::string>("left"),
        params.Get<std::string>("right"),
        params.Get<std::string>("bottom"),
        params.Get<std::string>("top"),
        params.Get<std::string>("front"),
        params.Get<std::string>("back"),
    };
    
    Generate(faces);
    
    return m_id != 0;
}

unsigned Cubemap::Generate(const std::vector<std::string> &faces)
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
    
    for (int i = 0; i < faces.size(); i++)
    {
        Image *image = app->assets->GetImage(faces[i]);
        if (image)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image->GetWidth(), image->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->GetPixels());
            delete image;
        }
        else
        {
            ERROR("Cubemap texture failed to load: " + faces[i]);
        }
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return m_id;
}

const std::string& Cubemap::GetName() const
{
    return m_name;
}

unsigned Cubemap::GetID() const
{
    return m_id;
}
