
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
#include "Model.h"
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

            void BeginRender();
            void EndRender();
        
            void ApplyRenderOptions(int options) const;
            void RevertRenderOptions(int options) const;
        
            void Render(const Renderable *renderable, const Transform *transform, const Material *material, int options = 0) const;
        
        private:
            friend class Application;
            Renderer();
        
            Color clearColor;
        
            Camera *camera;
            std::vector<Light*> lights;
        
            Matrix4x4 viewMatrix;
            Matrix4x4 projectionMatrix;
            Matrix4x4 viewProjectionMatrix;
    };

    class RenderOption
    {
        public:
            static const int RENDER_BACKFACE = 1 << 0;
            static const int NO_DEPTH_WRITE = 1 << 1;
            static const int NO_DEPTH_TEST = 1 << 2;
    };
}

#endif
