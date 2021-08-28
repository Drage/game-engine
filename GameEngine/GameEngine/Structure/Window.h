
#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <string>

namespace DrageEngine
{
    class Window
    {
        public:
            int GetWidth() const;
            int GetHeight() const;
            float GetAspectRatio() const;
            void SetSize(int width, int height);
        
            void EnableFullscreen();
            void DisableFullscreen();
            bool IsFullscreen() const;
        
            bool VSync() const;
            int GetRefreshRate() const;
        
        private:
            friend class Application;
        
            Window();
            ~Window();
            bool Create(int width, int height, bool fullscreen, const std::string &caption);
            void SwapBuffers();
        
            static int HandleWindowEvents(void* data, SDL_Event* event);
            void HandleResize();
        
            int m_flags;
            int m_width;
            int m_height;
            int m_scaleFactor;
            bool m_fullscreen;
            std::string m_caption;
            bool m_vsync;
            int m_refreshRate;
        
            SDL_Window *m_window;
            SDL_GLContext m_glContext;
    };
}

#endif
