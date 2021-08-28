
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
    camera = NULL;
    
    clearColor = Color(0.161f, 0.169f, 0.204f);
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    
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
    clearColor = color;
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}

const Color& Renderer::GetClearColor() const
{
    return clearColor;
}

void Renderer::SetActiveCamera(Camera *camera)
{
    this->camera = camera;
}

Camera* Renderer::GetActiveCamera()
{
    return camera;
}

void Renderer::AddLight(Light *light)
{
    for (std::vector<Light*>::iterator i = lights.begin(); i != lights.end(); i++)
    {
        if (*i == light)
            return;
    }
    lights.push_back(light);
}

bool Renderer::RemoveLight(Light *light)
{
    for (std::vector<Light*>::iterator i = lights.begin(); i != lights.end(); i++)
    {
        if (*i == light)
        {
            lights.erase(i);
            return true;
        }
    }
    return false;
}

void Renderer::BeginRender()
{
    static bool noCamErrorShown = false;
    if (!camera)
    {
        if (!noCamErrorShown)
        {
            noCamErrorShown = true;
            ERROR("Unable to render - no camera set");
        }
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    projectionMatrix = camera->GetProjectionMatrix();
    viewMatrix = camera->GetViewMatrix();
    viewProjectionMatrix = projectionMatrix * viewMatrix;
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
    
    shader->SetUniform("cameraPosition", camera->GetPosition());
    shader->SetUniform("numLights", (int)lights.size());
    for (int i = 0; i < lights.size(); i++)
        lights[i]->ApplyUniforms(shader, i);
    
    shader->SetUniform("time", Time::RunTime());
    shader->SetUniform("modelMatrix", modelMatrix);
    shader->SetUniform("viewMatrix", viewMatrix);
    shader->SetUniform("projectionMatrix", projectionMatrix);
    shader->SetUniform("modelViewMatrix", viewMatrix * modelMatrix);
    shader->SetUniform("viewProjectionMatrix", viewProjectionMatrix);
    shader->SetUniform("modelViewProjectionMatrix", viewProjectionMatrix * modelMatrix);
    
    ApplyRenderOptions(options);
    
    renderable->Render();
    
    RevertRenderOptions(options);
    glUseProgram(NULL);
}
