
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
#include "Editor.h"

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
            Editor *editor;

            const Scene* GetActiveScene() const;
            void SetActiveScene(Scene *scene);
        
            bool IsInEditMode() const;
        
        private:
            bool init;
            bool quit;
            int fps;
            Scene *activeScene;
            bool editMode;
    };

    extern Application *app;
}

#endif
