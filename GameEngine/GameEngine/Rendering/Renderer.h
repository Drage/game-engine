
#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "Camera.h"
#include "Color.h"
#include "Transform.h"
#include "Texture.h"
#include "Shader.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Terrain.h"
#include "Renderable.h"

namespace DrageEngine
{
    class Renderer
    {
        public:        
            void ViewportResized(int width, int height);
        
            void SetClearColor(const Color &color);
            const Color& GetClearColor() const;
        
            void SetActiveCamera(Camera *camera);
            Camera* GetActiveCamera();
        
            void AddLight(Light *light);
            bool RemoveLight(Light *light);
        
            void Register(Renderable *renderable);
            void Unregister(Renderable *renderable);
        
            void Render();
        
            Entity* GetEntityAtScreenPosition(const Vector2 &coordinates);
        
        private:
            friend class Application;
        
            typedef std::vector<Renderable*> RenderQueue;
            typedef bool (Renderer::*RenderFilterFunc)(RenderQueue::const_iterator i) const;
            typedef void (Renderer::*RenderCallbackFunc)(RenderQueue::const_iterator i) const;
        
            Renderer();
            void InitEditorSelection();
        
            void SortRenderQueue();
            void RenderPass(RenderFilterFunc filter = NULL, RenderCallbackFunc preRender = NULL);
            bool FilterInFrustum(RenderQueue::const_iterator i) const;
            void SetDefaultUniforms(const Shader* shader) const;
            void SetModelUniforms(const Shader* shader, const Renderable *renderable) const;
            void ApplyRenderOptions(const Renderable *renderable) const;
            void RenderMesh(const Mesh *mesh) const;
        
            void RenderEditorSelection();
            bool FilterIsSelected(RenderQueue::const_iterator i) const;
            void OverrideColorBlack(RenderQueue::const_iterator i) const;
            void OverrideColorFromIndex(RenderQueue::const_iterator i) const;
        
            Color clearColor;
            int viewportWidth;
            int viewportHeight;
        
            Camera *camera;
            std::vector<Light*> lights;
        
            RenderQueue renderQueue;
        
            Matrix4x4 viewMatrix;
            Matrix4x4 projectionMatrix;
            Matrix4x4 viewProjectionMatrix;
            Matrix4x4 orthoProjectionMatrix;
        
            unsigned editorSelectionFbo;
            unsigned editorSelectionTexture;
            unsigned editorSelectionDepth;
            unsigned editorSelectionVbo;
            Shader* editorSelectionShader;
        
            int renderPasses = 0;
            int drawCalls = 0;
    };

    class RenderOption
    {
        public:
            static const unsigned RENDER_BACKFACE = 1 << 0;
            static const unsigned NO_DEPTH_WRITE = 1 << 1;
            static const unsigned NO_DEPTH_TEST = 1 << 2;
            static const unsigned NO_FRUSTUM_CULL = 1 << 3;
            static const unsigned FRUSTUM_CULL_SPHERE = 1 << 4;
            static const unsigned NON_SELECTABLE = 1 << 5;
    };
}

#endif
