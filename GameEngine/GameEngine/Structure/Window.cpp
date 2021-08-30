
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include "Window.h"
#include "Application.h"

using namespace DrageEngine;

Window::Window()
{
    flags = 0;
    width = 0;
    height = 0;
    fullscreen = false;
    vsync = true;
}

Window::~Window()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Window::Create(int width, int height, bool fullscreen, const std::string &caption)
{
    flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
    this->width = width;
    this->height = height;
    this->fullscreen = fullscreen;
    this->caption = caption;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        ERROR("Failed to initialize the SDL2 library");
        return false;
    }
    
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    // Create application window
    int x = SDL_WINDOWPOS_UNDEFINED, y = SDL_WINDOWPOS_UNDEFINED;
    window = SDL_CreateWindow(caption.c_str(), x, y, width, height, flags);
    if (!window)
    {
        ERROR("Failed to create window");
        return false;
    }
    
    glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        ERROR("Failed to create the openGL context");
        return false;
    }
    
    // Fix window size if not what we asked for
    int drawableWidth, drawableHeight;
    SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);
    scaleFactor = drawableHeight / height;
    SDL_SetWindowSize(window, width / scaleFactor, height / scaleFactor);
    
    // Use Vsync
    if (SDL_GL_SetSwapInterval(vsync) < 0)
    {
        WARNING("Unable to set VSync");
        WARNING(SDL_GetError());
    }
    
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(0, 0, &mode);
    refreshRate = mode.refresh_rate;
    
    // Listen for resize events
    SDL_AddEventWatch(HandleWindowEvents, window);
    
    return true;
}

void Window::SwapBuffers()
{
    SDL_GL_SwapWindow(window);
}

int Window::HandleWindowEvents(void* data, SDL_Event* event)
{
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED)
    {
        app->window->HandleResize();
        return 0;
    }
    return 1;
}

void Window::HandleResize()
{
    SDL_GetWindowSize(window, &width, &height);
    width *= scaleFactor;
    height *= scaleFactor;
    app->renderer->ViewportResized(width, height);
}

int Window::GetWidth() const
{
    return width;
}

int Window::GetHeight() const
{
    return height;
}

float Window::GetAspectRatio() const
{
    return 1.0f * width / height;
}

int Window::GetScaleFactor() const
{
    return scaleFactor;
}

void Window::SetSize(int width, int height)
{
    width = width;
    height = height;
    app->renderer->ViewportResized(width, height);
}

void Window::EnableFullscreen()
{
    if (!fullscreen)
    {
        fullscreen = true;
        flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
        SDL_SetWindowFullscreen(window, flags);
    }
}

void Window::DisableFullscreen()
{
    if (fullscreen)
    {
        fullscreen = false;
        flags = flags - SDL_WINDOW_FULLSCREEN_DESKTOP;
        SDL_SetWindowFullscreen(window, flags);
    }
}

bool Window::IsFullscreen() const
{
    return fullscreen;
}

bool Window::VSync() const
{
    return vsync;
}

int Window::GetRefreshRate() const
{
    return refreshRate;
}
