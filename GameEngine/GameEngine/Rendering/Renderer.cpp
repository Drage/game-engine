
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
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
    
    if (app->IsInEditMode())
        InitEditorSelection();
}

void Renderer::InitEditorSelection()
{
    editorSelectionShader = app->assets->GetShader("EditorSelection.shader");
    
    glGenTextures(1, &editorSelectionTexture);
    glBindTexture(GL_TEXTURE_2D, editorSelectionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, app->window->GetWidth(), app->window->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glGenTextures(1, &editorSelectionDepth);
    glBindTexture(GL_TEXTURE_2D, editorSelectionDepth);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, app->window->GetWidth(), app->window->GetHeight(), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
    
    glGenFramebuffers(1, &editorSelectionFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, editorSelectionFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, editorSelectionTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, editorSelectionDepth, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glGenVertexArrays(1, &editorSelectionVbo);
}

void Renderer::ViewportResized(int width, int height)
{
    glViewport(0, 0, width, height);
    
    glBindTexture(GL_TEXTURE_2D, editorSelectionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, app->window->GetWidth(), app->window->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, editorSelectionDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, app->window->GetWidth(), app->window->GetHeight(), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
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

void Renderer::Register(Renderable *renderable)
{
    renderQueue.push_back(renderable);
}

void Renderer::Unregister(Renderable *renderable)
{
    for (RenderQueue::iterator i = renderQueue.begin(); i != renderQueue.end(); i++)
    {
        if (*i == renderable)
        {
            renderQueue.erase(i);
            return;
        }
    }
}

void Renderer::Render()
{
    if (!camera)
    {
        ERROR("Unable to render - no camera set");
        return;
    }
    viewMatrix = camera->GetViewMatrix();
    projectionMatrix = camera->GetProjectionMatrix();
    viewProjectionMatrix = projectionMatrix * viewMatrix;

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for (RenderQueue::const_iterator i = renderQueue.begin(); i != renderQueue.end(); i++)
    {
        Vector3 position = (*i)->GetEntity()->GetGlobalTransform().position;
        float depthSqr = (position - camera->GetPosition()).MagnitudeSqr();
        bool isTransparent = (*i)->GetMaterial()->IsTransparent();
        (*i)->SetDepthOrder(isTransparent ? depthSqr : -depthSqr);
    }
    
    RenderableComparitor renderableComparitor;
    std::sort(renderQueue.begin(), renderQueue.end(), renderableComparitor);
    
    RenderPass();
    
    if (app->IsInEditMode())
        RenderEditorSelection();
}

void Renderer::RenderPass(RenderFilterFunc filter, RenderCallbackFunc preRender) const
{
    const Shader *activeShader = NULL;
    const Material *activeMaterial = NULL;
    const Mesh *activeMesh = NULL;
    
    for (RenderQueue::const_iterator i = renderQueue.begin(); i != renderQueue.end(); i++)
    {
        const Renderable* renderable = (*i);
        
        if (filter && !(this->*filter)(i))
            continue;
        
        const Shader *shader = renderable->GetMaterial()->GetShader();
        if (activeShader != shader)
        {
            activeShader = shader;
            glUseProgram(shader->GetID());
            SetDefaultUniforms(shader);
        }
        
        const Material *material = renderable->GetMaterial();
        if (activeMaterial != material)
        {
            activeMaterial = material;
            material->ApplyUniforms();
        }
        
        const Mesh *mesh = renderable->GetMesh();
        if (activeMesh != mesh)
        {
            activeMesh = mesh;
            glBindVertexArray(mesh->GetID());
        }
        
        SetModelUniforms(shader, renderable);
        ApplyRenderOptions(renderable->GetOptions());
        
        if (preRender)
            (this->*preRender)(i);
            
        RenderMesh(mesh);
    }
}

void Renderer::SetDefaultUniforms(const Shader* shader) const
{
    shader->SetUniform("time", Time::RunTime());
    
    shader->SetUniform("cameraPosition", camera->GetPosition());
    shader->SetUniform("viewMatrix", viewMatrix);
    shader->SetUniform("projectionMatrix", projectionMatrix);
    shader->SetUniform("viewProjectionMatrix", viewProjectionMatrix);
    
    shader->SetUniform("numLights", (int)lights.size());
    for (int i = 0; i < lights.size(); i++)
    {
        shader->SetUniform("lights", i, "position", lights[i]->position);
        shader->SetUniform("lights", i, "intensities", lights[i]->color);
        shader->SetUniform("lights", i, "attenuation", lights[i]->attenuation);
        shader->SetUniform("lights", i, "ambientCoefficient", lights[i]->ambientCoefficient);
        shader->SetUniform("lights", i, "coneDirection", lights[i]->coneDirection);
        shader->SetUniform("lights", i, "coneAngle", lights[i]->coneAngle);
    }
}

void Renderer::SetModelUniforms(const Shader* shader, const Renderable *renderable) const
{
    Matrix4x4 modelMatrix = renderable->GetEntity()->GetGlobalTransform().ToMatrix();
    shader->SetUniform("modelMatrix", modelMatrix);
    shader->SetUniform("modelViewMatrix", viewMatrix * modelMatrix);
    shader->SetUniform("modelViewProjectionMatrix", viewProjectionMatrix * modelMatrix);
}

void Renderer::ApplyRenderOptions(unsigned options) const
{
    glCullFace(options & RenderOption::RENDER_BACKFACE ? GL_FRONT : GL_BACK);
    glDepthMask(options & RenderOption::NO_DEPTH_WRITE ? GL_FALSE : GL_TRUE);
    
    if (options & RenderOption::NO_DEPTH_TEST)
        glDisable(GL_DEPTH_TEST);
    else
        glEnable(GL_DEPTH_TEST);
}

void Renderer::RenderMesh(const Mesh *mesh) const
{
    if (mesh->HasIndices())
        glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, NULL);
    else
        glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertexCount());
}

void Renderer::RenderEditorSelection() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, editorSelectionFbo);
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &drawBuffer);
    
    RenderPass(&Renderer::RenderableIsSelected, &Renderer::OverrideColorBlack);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Shader *shader = editorSelectionShader;
    glUseProgram(editorSelectionShader->GetID());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, editorSelectionTexture);
    shader->SetUniform("silhouette", 0);
    shader->SetUniform("width", shader->GetDefaultUniformValues().Get<int>("width"));
    shader->SetUniform("color", shader->GetDefaultUniformValues().Get<Vector4>("color"));
    glBindVertexArray(editorSelectionVbo);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

bool Renderer::RenderableIsSelected(RenderQueue::const_iterator i) const
{
    return app->editor->IsSelected((*i)->GetEntity());
}

void Renderer::OverrideColorBlack(RenderQueue::const_iterator i) const
{
    const Shader *shader = (*i)->GetMaterial()->GetShader();
    shader->SetUniform("colorOverride", Vector4(0, 0, 0, 1));
    glDepthMask(GL_TRUE);
}

void Renderer::OverrideColorFromIndex(RenderQueue::const_iterator i) const
{
    int index = (int)std::distance(renderQueue.begin(), i);
    int r = (index & 0x000000FF) >>  0;
    int g = (index & 0x0000FF00) >>  8;
    int b = (index & 0x00FF0000) >> 16;
    const Shader *shader = (*i)->GetMaterial()->GetShader();
    shader->SetUniform("colorOverride", Vector4(r/255.0f, g/255.0f, b/255.0f, 1.0f));
}

Entity* Renderer::GetEntityAtScreenPosition(const Vector2 &coordinates) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, editorSelectionFbo);
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    RenderPass(NULL, &Renderer::OverrideColorFromIndex);
    
    glFinish();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned char data[4];
    glReadPixels(coordinates.x, app->window->GetHeight() - coordinates.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    int index = data[0] + data[1] * 256 + data[2] * 256*256;
    if (index < renderQueue.size())
    {
        RenderQueue::const_iterator it = renderQueue.begin();
        std::advance(it, index);
        return (*it)->GetEntity();
    }
    return NULL;
}
