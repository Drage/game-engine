
#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>

namespace DrageEngine
{
    class Window
    {
        public:
            int GetWidth() const;
            int GetHeight() const;
            float GetAspectRatio() const;
            int GetScaleFactor() const;
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
        
            int flags;
            int width;
            int height;
            int scaleFactor;
            bool fullscreen;
            std::string caption;
            bool vsync;
            int refreshRate;
        
            SDL_Window *window;
            SDL_GLContext glContext;
    };
}

#endif
