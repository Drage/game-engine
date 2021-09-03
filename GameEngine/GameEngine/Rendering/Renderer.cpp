
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
    
    editorSelectionMode = false;
    editorSelectionShader = app->assets->GetShader("EditorSelection.shader");
    
    glGenTextures(1, &selectedObjectsMaskTexture);
    glBindTexture(GL_TEXTURE_2D, selectedObjectsMaskTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, app->window->GetWidth(), app->window->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glGenFramebuffers(1, &selectedObjectsMaskFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, selectedObjectsMaskFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, selectedObjectsMaskTexture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::ViewportResized(int width, int height)
{
    glViewport(0, 0, width, height);
    
    glBindTexture(GL_TEXTURE_2D, selectedObjectsMaskTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, app->window->GetWidth(), app->window->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
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
    renderQueue.insert(renderable);
}

void Renderer::Unregister(Renderable *renderable)
{
    renderQueue.erase(renderable);
}

void Renderer::Render() const
{
    if (!camera)
    {
        ERROR("Unable to render - no camera set");
        return;
    }

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Matrix4x4 viewMatrix = camera->GetViewMatrix();
    Matrix4x4 projectionMatrix = camera->GetProjectionMatrix();
    Matrix4x4 viewProjectionMatrix = projectionMatrix * viewMatrix;

    const Shader *activeShader = NULL;
    const Material *activeMaterial = NULL;
    const Mesh *activeMesh = NULL;
    
    if (app->IsInEditMode())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, selectedObjectsMaskFbo);
        glClearColor(1, 1, 1, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    for (RenderQueue::const_iterator i = renderQueue.begin(); i != renderQueue.end(); i++)
    {
        const Renderable* renderable = (*i);
        
        const Shader *shader = renderable->GetMaterial()->GetShader();
        if (activeShader != shader)
        {
            activeShader = shader;
            glUseProgram(shader->GetID());
            
            shader->SetUniform("time", Time::RunTime());
            shader->SetUniform("cameraPosition", camera->GetPosition());
            shader->SetUniform("viewMatrix", viewMatrix);
            shader->SetUniform("projectionMatrix", projectionMatrix);
            shader->SetUniform("viewProjectionMatrix", viewProjectionMatrix);
            shader->SetUniform("numLights", (int)lights.size());
            for (int i = 0; i < lights.size(); i++)
                lights[i]->ApplyUniforms(shader, i);
        }
        
        const Material *material = renderable->GetMaterial();
        if (activeMaterial != material)
        {
            activeMaterial = material;
            material->ApplyUniforms();
        }
        
        Matrix4x4 modelMatrix = renderable->GetEntity()->GetGlobalTransform().ToMatrix();
        shader->SetUniform("modelMatrix", modelMatrix);
        shader->SetUniform("modelViewMatrix", viewMatrix * modelMatrix);
        shader->SetUniform("modelViewProjectionMatrix", viewProjectionMatrix * modelMatrix);
        
        unsigned options = renderable->GetOptions();
        ApplyRenderOptions(options);
        
        const Mesh *mesh = renderable->GetMesh();
        if (activeMesh != mesh)
        {
            activeMesh = mesh;
            glBindVertexArray(mesh->GetID());
        }
        RenderMesh(mesh);
        
        if (app->IsInEditMode() && app->editor->IsSelected(renderable->GetEntity()))
        {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, selectedObjectsMaskFbo);
            GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
            glDrawBuffers(1, &drawBuffer);
            shader->SetUniform("editorSelectionStencil", true);
            RenderMesh(mesh);
            shader->SetUniform("editorSelectionStencil", false);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        }
    }
    
    if (app->IsInEditMode())
    {
        Shader *shader = editorSelectionShader;
        glUseProgram(shader->GetID());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, selectedObjectsMaskTexture);
        shader->SetUniform("silhouette", 0);
        shader->SetUniform("width", shader->GetDefaultUniformValues().Get<int>("width"));
        shader->SetUniform("color", shader->GetDefaultUniformValues().Get<Vector4>("color"));
        unsigned vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
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

Entity* Renderer::GetEntityAtScreenPosition(const Vector2 &coordinates) const
{
    return NULL;
}

//void Renderer::BeginRender()
//{
//    static bool noCamErrorShown = false;
//    if (!camera)
//    {
//        if (!noCamErrorShown)
//        {
//            noCamErrorShown = true;
//            ERROR("Unable to render - no camera set");
//        }
//        return;
//    }
//
//    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    projectionMatrix = camera->GetProjectionMatrix();
//    viewMatrix = camera->GetViewMatrix();
//    viewProjectionMatrix = projectionMatrix * viewMatrix;
//
////    glBindFramebuffer(GL_FRAMEBUFFER, selectedObjectsMaskFbo);
////    glClearColor(1, 1, 1, 0);
////    glClear(GL_COLOR_BUFFER_BIT);
////    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}

//void Renderer::Render(const Mesh *mesh, const Transform *transform, const Material *material, int options) const
//{
//    Matrix4x4 modelMatrix = transform->ToMatrix();
//
//    Shader *shader = material->GetShader();
//    glUseProgram(shader->GetID());
//
//    material->ApplyUniforms();
//
//    shader->SetUniform("cameraPosition", camera->GetPosition());
//    shader->SetUniform("numLights", (int)lights.size());
//    for (int i = 0; i < lights.size(); i++)
//        lights[i]->ApplyUniforms(shader, i);
//
//    shader->SetUniform("time", Time::RunTime());
//    shader->SetUniform("modelMatrix", modelMatrix);
//    shader->SetUniform("viewMatrix", viewMatrix);
//    shader->SetUniform("projectionMatrix", projectionMatrix);
//    shader->SetUniform("modelViewMatrix", viewMatrix * modelMatrix);
//    shader->SetUniform("viewProjectionMatrix", viewProjectionMatrix);
//    shader->SetUniform("modelViewProjectionMatrix", viewProjectionMatrix * modelMatrix);
//
//    shader->SetUniform("editorSelectMode", editorSelectionMode);
//    if (editorSelectionMode)
//    {
//        int r = (currentSceneIndex & 0x000000FF) >>  0;
//        int g = (currentSceneIndex & 0x0000FF00) >>  8;
//        int b = (currentSceneIndex & 0x00FF0000) >> 16;
//        shader->SetUniform("editorIndexColor", Vector4(r/255.0f, g/255.0f, b/255.0f, 1.0f));
//    }
//
//    if (app->IsInEditMode())
//    {
//        const std::vector<int> selection = app->editor->GetSelectionSceneIndexes();
//        if (std::find(selection.begin(), selection.end(), currentSceneIndex) != selection.end())
//        {
//            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, selectedObjectsMaskFbo);
//
//            GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
//            glDrawBuffers(1, &drawBuffer);
//
//            shader->SetUniform("editorSelectionStencil", true);
//            mesh->Render();
//            shader->SetUniform("editorSelectionStencil", false);
//
//            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//        }
//    }
//
//    ApplyRenderOptions(options);
//
//    mesh->Render();
//
//    RevertRenderOptions(options);
//    glUseProgram(NULL);
//}

//void Renderer::EndRender()
//{
////    Shader *shader = editorSelectionShader;
////    glUseProgram(shader->GetID());
////
////    shader->SetUniform("silhouette", 0);
////    glActiveTexture(GL_TEXTURE0);
////    glBindTexture(GL_TEXTURE_2D, selectedObjectsMaskTexture);
////
////    shader->SetUniform("width", shader->GetDefaultUniformValues().Get<int>("width"));
////    shader->SetUniform("color", shader->GetDefaultUniformValues().Get<Vector4>("color"));
////
////    unsigned vao;
////    glGenVertexArrays(1, &vao);
////    glBindVertexArray(vao);
////    glDrawArrays(GL_TRIANGLES, 0, 6);
//
//    glFlush();
//}

void Renderer::SetEditorSelectionModeEnabled(bool enabled)
{
    editorSelectionMode = enabled;
}

void Renderer::SetSceneIndex(int index)
{
    currentSceneIndex = index;
}

int Renderer::GetSceneObjectIndexAtScreenPosition(const Vector2 &coordinates)
{
    glFlush();
    glFinish();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned char data[4];
    glReadPixels(coordinates.x, app->window->GetHeight() - coordinates.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    return data[0] + data[1] * 256 + data[2] * 256*256;
}
