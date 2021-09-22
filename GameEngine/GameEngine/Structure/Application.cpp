
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include <iostream>
#include "Application.h"
#include "Debug.h"
#include "Timer.h"
#include "Editor.h"

using namespace DrageEngine;

Application *DrageEngine::app = new Application();

Application::Application()
{
    if (std::getenv("EDITOR") != NULL)
    {
        editorEnabled = true;
        editMode = true;
    }
}
 
Application::~Application()
{
    delete window;
    delete renderer;
    delete assets;
    delete input;
}

bool Application::Init()
{
    window = new Window(1440 / 1.5, 900 / 1.5);
    assets = new AssetManager();
    renderer = new Renderer();
    input = new Input();
    audio = new AudioManager();
    
    if (editorEnabled)
        editor = new Editor();
    
    return window->IsInitialized();
}

void Application::Run(Game *game)
{
    game->Load();
    
    if (!editMode)
        game->Start();
    
    Time::Init();

    while (!quit)
    {
        input->Clear();
        
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            // Clicked on close button or press escape
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
                quit = true;
        }
        
        Time::Update();
        
        game->Update();
        
        if (editMode)
            editor->Update();

        renderer->Render();
        
        if (editMode)
            editor->Render();
        
        window->SwapBuffers();
    }
}

void Application::Quit()
{
    quit = true;
}

const Scene* Application::GetActiveScene() const
{
    return activeScene;
}

void Application::SetActiveScene(Scene *scene)
{
    activeScene = scene;
}

bool Application::IsEditorEnabled() const
{
    return editorEnabled;
}

bool Application::IsInEditMode() const
{
    return editMode;
}
