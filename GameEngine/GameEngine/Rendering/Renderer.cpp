
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "Renderer.h"
#include "Primitives.h"
#include "Debug.h"
#include "AssetManager.h"
#include "Application.h"

using namespace DrageEngine;

Renderer::Renderer()
{
    m_camera = NULL;
    
    m_clearColor = Color(0.161f, 0.169f, 0.204f);
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    
    ViewportResized(app->window->GetWidth(), app->window->GetHeight());
}

void Renderer::ViewportResized(int width, int height)
{
    glViewport(0, 0, width, height);
}

void Renderer::SetClearColor(const Color &color)
{
    m_clearColor = color;
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
}

const Color& Renderer::GetClearColor() const
{
    return m_clearColor;
}

void Renderer::SetActiveCamera(Camera *camera)
{
    m_camera = camera;
}

Camera* Renderer::GetActiveCamera()
{
    return m_camera;
}

void Renderer::AddLight(Light *light)
{
    for (std::vector<Light*>::iterator i = m_lights.begin(); i != m_lights.end(); i++)
    {
        if (*i == light)
            return;
    }
    m_lights.push_back(light);
}

bool Renderer::RemoveLight(Light *light)
{
    for (std::vector<Light*>::iterator i = m_lights.begin(); i != m_lights.end(); i++)
    {
        if (*i == light)
        {
            m_lights.erase(i);
            return true;
        }
    }
    return false;
}

void Renderer::BeginRender()
{
    static bool noCamErrorShown = false;
    if (!m_camera)
    {
        if (!noCamErrorShown)
        {
            noCamErrorShown = true;
            ERROR("Unable to render - no camera set");
        }
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_projectionMatrix = m_camera->GetProjectionMatrix();
    m_viewMatrix = m_camera->GetViewMatrix();
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void Renderer::EndRender()
{

}

void Renderer::ApplyRenderOptions(int options) const
{
    if (options & RenderOption::RENDER_BACKFACE)
        glCullFace(GL_FRONT);
    
    if (options & RenderOption::NO_DEPTH_WRITE)
        glDepthMask(GL_FALSE);
    
    if (options & RenderOption::NO_DEPTH_TEST)
        glDisable(GL_DEPTH_TEST);
}

void Renderer::RevertRenderOptions(int options) const
{
    if (options & RenderOption::RENDER_BACKFACE)
        glCullFace(GL_BACK);
    
    if (options & RenderOption::NO_DEPTH_WRITE)
        glDepthMask(GL_TRUE);
    
    if (options & RenderOption::NO_DEPTH_TEST)
        glEnable(GL_DEPTH_TEST);
}

void Renderer::Render(const Renderable *renderable, const Transform *transform, const Material *material, int options) const
{
    Matrix4x4 modelMatrix = transform->ToMatrix();
    
    Shader *shader = material->GetShader();
    glUseProgram(shader->GetID());

    material->ApplyUniforms();
    
    shader->SetUniform("cameraPosition", m_camera->GetPosition());
    shader->SetUniform("numLights", (int)m_lights.size());
    for (int i = 0; i < m_lights.size(); i++)
        m_lights[i]->ApplyUniforms(shader, i);
    
    shader->SetUniform("time", Time::RunTime());
    shader->SetUniform("modelMatrix", modelMatrix);
    shader->SetUniform("viewMatrix", m_viewMatrix);
    shader->SetUniform("projectionMatrix", m_projectionMatrix);
    shader->SetUniform("modelViewMatrix", m_viewMatrix * modelMatrix);
    shader->SetUniform("viewProjectionMatrix", m_viewProjectionMatrix);
    shader->SetUniform("modelViewProjectionMatrix", m_viewProjectionMatrix * modelMatrix);
    
    ApplyRenderOptions(options);
    
    renderable->Render();
    
    RevertRenderOptions(options);
    glUseProgram(NULL);
}
