
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
    m_init = false;
    m_fps = 60;
    editMode = true;
    window = new Window();
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
    if (!window->Create(1280, 720, false, ""))
        return false;

    renderer = new Renderer();
    assets = new AssetManager();
    input = new Input();
    audio = new AudioManager();
    editor = new Editor();
    
    m_init = true;
    return true;
}

bool Application::EditorInit()
{
    renderer = new Renderer();
    assets = new AssetManager();
    input = new Input();
    audio = new AudioManager();
    
    m_init = true;
    return true;
}

void Application::Run(Game *game)
{
    game->Load();
    
    if (!editMode)
        game->Start();
    
    Time::Init();

    while (!m_quit)
    {
        input->Clear();
        
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // Clicked on close button or press escape
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
                m_quit = true;
        }
        
        Time::Update();
        
        if (!editMode)
            game->Update();
        else
            editor->Update();

        renderer->BeginRender();
        game->Render(renderer);
        renderer->EndRender();
        window->SwapBuffers();
    }
}

void Application::Quit()
{
    m_quit = true;
}

const Scene* Application::GetActiveScene() const
{
    return m_activeScene;
}

void Application::SetActiveScene(Scene *scene)
{
    m_activeScene = scene;
}
