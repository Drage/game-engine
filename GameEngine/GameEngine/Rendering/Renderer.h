
#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <set>
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
        
            void Render() const;
        
            Entity* GetEntityAtScreenPosition(const Vector2 &coordinates) const;
        
            void SetEditorSelectionModeEnabled(bool enabled);
            void SetSceneIndex(int index);
            int GetSceneObjectIndexAtScreenPosition(const Vector2 &coordinates);
        
        private:
            friend class Application;
            Renderer();

            void ApplyRenderOptions(unsigned options) const;
            void RenderMesh(const Mesh *mesh) const;
        
            Color clearColor;
        
            Camera *camera;
            std::vector<Light*> lights;
        
            typedef std::multiset<Renderable*, CmpRenderablePtrs> RenderQueue;
            RenderQueue renderQueue;
        
            bool editorSelectionMode;
            int currentSceneIndex;
        
            unsigned selectedObjectsMaskFbo;
            unsigned selectedObjectsMaskTexture;
            unsigned selectedObjectsMaskDepthBuffer;
            Shader* editorSelectionShader;
    };

    class RenderOption
    {
        public:
            static const unsigned RENDER_BACKFACE = 1 << 0;
            static const unsigned NO_DEPTH_WRITE = 1 << 1;
            static const unsigned NO_DEPTH_TEST = 1 << 2;
    };
}

#endif
