
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
#include "MathUtils.h"

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
    
    ViewportResized(app->window->GetDrawableWidth(), app->window->GetDrawableHeight());
    
    if (app->IsInEditMode())
        InitEditorSelection();
}

void Renderer::InitEditorSelection()
{
    editorSelectionShader = app->assets->GetShader("EditorSelection.shader");
    
    glGenTextures(1, &editorSelectionTexture);
    glBindTexture(GL_TEXTURE_2D, editorSelectionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewportSize.x, viewportSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, viewportSize.x, viewportSize.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
    
    glGenFramebuffers(1, &editorSelectionFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, editorSelectionFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, editorSelectionTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, editorSelectionDepth, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glGenVertexArrays(1, &editorSelectionVbo);
}

void Renderer::ViewportResized(int width, int height, int offsetX, int offsetY)
{
    if (width == 0 || height == 0)
    {
        width = viewportSize.x;
        height = viewportSize.y;
        offsetX = viewportOffset.x;
        offsetY = viewportOffset.y;
    }
    
    if (viewportSize.x != width || viewportSize.y != height || viewportOffset.x != offsetX || viewportOffset.y != offsetY)
    {
        viewportSize = Vector2(width, height);
        viewportOffset = Vector2(offsetX, offsetY);
        
        glBindTexture(GL_TEXTURE_2D, editorSelectionTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewportSize.x, viewportSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glBindTexture(GL_TEXTURE_2D, editorSelectionDepth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, viewportSize.x, viewportSize.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
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

const Vector2& Renderer::GetViewportSize() const
{
    return viewportSize;
}

const Vector2& Renderer::GetViewportOffset() const
{
    return viewportOffset;
}

float Renderer::GetViewportAspectRatio() const
{
    return viewportSize.x / viewportSize.y;
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
    for (auto i : lights)
    {
        if (i == light)
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

void Renderer::GetRenderablesForEntity(const Entity *entity, std::vector<Renderable*> &renderables) const
{
    for (auto renderable : renderQueue)
    {
        if (renderable->GetEntity() == entity)
            renderables.push_back(renderable);
    }
}

void Renderer::GetRenderablesForEntities(const std::vector<Entity*> &entities, std::vector<Renderable*> &renderables) const
{
    for (auto renderable : renderQueue)
    {
        if (std::find(entities.begin(), entities.end(), renderable->GetEntity()) != entities.end())
            renderables.push_back(renderable);
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
    orthoProjectionMatrix.SetProjectionOrthographic(viewportSize.x, viewportSize.y);

    glViewport(viewportOffset.x, viewportOffset.y, viewportSize.x, viewportSize.y);
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    renderPasses = 0;
    drawCalls = 0;
    
    SortRenderQueue();
    RenderPass(&Renderer::FilterInFrustum);
    
    if (app->IsInEditMode() && app->editor->HasSelection())
        RenderEditorSelection();
}

void Renderer::SortRenderQueue()
{
    for (auto renderable : renderQueue)
    {
        const Material *material = renderable->GetMaterial();
        Vector3 position = renderable->GetEntity()->GetGlobalTransform().position;
        
        if (material->IsUIOverlay())
            renderable->SetDepthOrder(1000 - position.z);
        else
        {
            bool isTransparent = renderable->GetMaterial()->IsTransparent();
            float depthSqr = (position - camera->GetPosition()).MagnitudeSqr();
            renderable->SetDepthOrder(isTransparent ? depthSqr : -depthSqr);
        }
    }
    
    RenderableComparitor renderableComparitor;
    std::sort(renderQueue.begin(), renderQueue.end(), renderableComparitor);
}

void Renderer::RenderPass(RenderFilterFunc filter, RenderCallbackFunc preRender)
{
    const Shader *activeShader = NULL;
    const Material *activeMaterial = NULL;
    const Mesh *activeMesh = NULL;
    
    renderPasses++;
    
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
        ApplyRenderOptions(renderable);
        
        if (preRender)
            (this->*preRender)(i);
            
        RenderMesh(mesh);
        drawCalls++;
    }
}

bool Renderer::FilterInFrustum(RenderQueue::const_iterator i) const
{
    Renderable *renderable = (*i);

    if (renderable->GetOptions() & RenderOption::NO_FRUSTUM_CULL)
        return true;
    
    if (renderable->GetOptions() & RenderOption::FRUSTUM_CULL_SPHERE)
    {
        Transform transform = renderable->GetEntity()->GetGlobalTransform();
        Vector3 position = transform.position;
        Vector3 scale = transform.scale;
        float scaleFactor = std::max({scale.x, scale.y, scale.z});
        float radius = renderable->GetMesh()->GetBounds().radius * scaleFactor;
        return camera->GetFrustum().CheckSphere(position, radius);
    }
    else
    {
        Bounds bounds = renderable->GetMesh()->GetBounds();
        Matrix4x4 modelMatrix = renderable->GetEntity()->GetGlobalTransform().ToMatrix();
        return camera->GetFrustum().CheckBounds(bounds, modelMatrix);
    }
}

void Renderer::SetDefaultUniforms(const Shader* shader) const
{
    shader->SetUniform("time", Time::RunTime());
    
    shader->SetUniform("cameraPosition", camera->GetPosition());
    shader->SetUniform("viewMatrix", viewMatrix);
    shader->SetUniform("projectionMatrix", projectionMatrix);
    shader->SetUniform("viewProjectionMatrix", viewProjectionMatrix);
    shader->SetUniform("orthoProjectionMatrix", orthoProjectionMatrix);
    
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

void Renderer::ApplyRenderOptions(const Renderable *renderable) const
{
    unsigned options = renderable->GetOptions();
    
    if (renderable->GetMaterial()->IsTransparent())
        options |= RenderOption::NO_DEPTH_WRITE;
    
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

void Renderer::RenderEditorSelection()
{
    glBindFramebuffer(GL_FRAMEBUFFER, editorSelectionFbo);
    glViewport(0, 0, viewportSize.x, viewportSize.y);
    glClearColor(1, 1, 1, 0);
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &drawBuffer);
    
    RenderPass(&Renderer::FilterIsSelected, &Renderer::OverrideColorBlack);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(viewportOffset.x, viewportOffset.y, viewportSize.x, viewportSize.y);
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

bool Renderer::FilterIsSelected(RenderQueue::const_iterator i) const
{
    const Entity *entity = (*i)->GetEntity();
    while (entity != NULL)
    {
        if (app->editor->IsSelected(entity))
            return true;
        entity = entity->GetParent();
    }
    return false;
}

void Renderer::OverrideColorBlack(RenderQueue::const_iterator i) const
{
    const Shader *shader = (*i)->GetMaterial()->GetShader();
    shader->SetUniform("colorOverride", Vector4(0, 0, 0, 1));
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

Entity* Renderer::GetEntityAtScreenPosition(const Vector2 &coordinates)
{
    glBindFramebuffer(GL_FRAMEBUFFER, editorSelectionFbo);
    glViewport(0, 0, viewportSize.x, viewportSize.y);
    glDepthMask(GL_TRUE);
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    RenderPass(NULL, &Renderer::OverrideColorFromIndex);
    
    glFinish();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned char data[4];
    float x = coordinates.x - viewportOffset.x;
    float y = app->window->GetDrawableHeight() - coordinates.y - viewportOffset.y;
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(viewportOffset.x, viewportOffset.y, viewportSize.x, viewportSize.y);
    
    int index = data[0] + data[1] * 256 + data[2] * 256 * 256;
    if (index < renderQueue.size())
    {
        RenderQueue::const_iterator i = renderQueue.begin();
        std::advance(i, index);
        Renderable *renderable = *i;
        
        if (!(renderable->GetOptions() & RenderOption::NON_SELECTABLE))
            return renderable->GetEntity();
    }
    return NULL;
}
