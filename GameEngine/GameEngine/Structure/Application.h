
#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include "Game.h"
#include "Renderer.h"
#include "Camera.h"
#include "AssetManager.h"
#include "Window.h"
#include "Input.h"
#include "AudioManager.h"
#include "Scene.h"
#include "GameTime.h"

namespace DrageEngine
{
    class Application
    {
        public:
            Application();
            ~Application();
        
            bool Init();
            bool EditorInit();
            void Run(Game *game);
            void Quit();
        
            Window *window;
            Renderer *renderer;
            AssetManager *assets;
            Input *input;
            AudioManager *audio;

            const Scene* GetActiveScene() const;
            void SetActiveScene(Scene *scene);
        
        private:
            static const bool EDITOR;
            bool m_init;
            bool m_quit;
            int m_fps;
            Scene *m_activeScene;
    };

    extern Application *app;
}

#endif
