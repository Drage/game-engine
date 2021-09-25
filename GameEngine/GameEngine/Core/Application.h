
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
        
            bool IsEditorEnabled() const;
            bool IsInEditMode() const;
        
        private:
            bool quit;
            Scene *activeScene;
            bool editorEnabled;
            bool editMode;
    };

    extern Application *app;
}

#endif
