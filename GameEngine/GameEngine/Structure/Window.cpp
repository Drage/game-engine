
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include "Window.h"
#include "Application.h"

using namespace DrageEngine;

Window::Window()
{
    m_flags = 0;
    m_width = 0;
    m_height = 0;
    m_fullscreen = false;
    m_vsync = true;
}

Window::~Window()
{
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool Window::Create(int width, int height, bool fullscreen, const std::string &caption)
{
    m_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
    m_width = width;
    m_height = height;
    m_fullscreen = fullscreen;
    m_caption = caption;
    
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
    m_window = SDL_CreateWindow(m_caption.c_str(), x, y, m_width, m_height, m_flags);
    if (!m_window)
    {
        ERROR("Failed to create window");
        return false;
    }
    
    m_glContext = SDL_GL_CreateContext(m_window);
    if (!m_glContext)
    {
        ERROR("Failed to create the openGL context");
        return false;
    }
    
    // Fix window size if not what we asked for
    int drawableWidth, drawableHeight;
    SDL_GL_GetDrawableSize(m_window, &drawableWidth, &drawableHeight);
    m_scaleFactor = drawableHeight / m_height;
    SDL_SetWindowSize(m_window, m_width / m_scaleFactor, m_height / m_scaleFactor);
    
    // Use Vsync
    if (SDL_GL_SetSwapInterval(m_vsync) < 0)
    {
        WARNING("Unable to set VSync");
        WARNING(SDL_GetError());
    }
    
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(0, 0, &mode);
    m_refreshRate = mode.refresh_rate;
    
    // Listen for resize events
    SDL_AddEventWatch(HandleWindowEvents, m_window);
    
    return true;
}

void Window::SwapBuffers()
{
    SDL_GL_SwapWindow(m_window);
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
    SDL_GetWindowSize(m_window, &m_width, &m_height);
    m_width *= m_scaleFactor;
    m_height *= m_scaleFactor;
    app->renderer->ViewportResized(m_width, m_height);
}

int Window::GetWidth() const
{
    return m_width;
}

int Window::GetHeight() const
{
    return m_height;
}

float Window::GetAspectRatio() const
{
    return 1.0f * m_width / m_height;
}

void Window::SetSize(int width, int height)
{
    m_width = width;
    m_height = height;
    app->renderer->ViewportResized(width, height);
}

void Window::EnableFullscreen()
{
    if (!m_fullscreen)
    {
        m_fullscreen = true;
        m_flags = m_flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
        SDL_SetWindowFullscreen(m_window, m_flags);
    }
}

void Window::DisableFullscreen()
{
    if (m_fullscreen)
    {
        m_fullscreen = false;
        m_flags = m_flags - SDL_WINDOW_FULLSCREEN_DESKTOP;
        SDL_SetWindowFullscreen(m_window, m_flags);
    }
}

bool Window::IsFullscreen() const
{
    return m_fullscreen;
}

bool Window::VSync() const
{
    return m_vsync;
}

int Window::GetRefreshRate() const
{
    return m_refreshRate;
}
