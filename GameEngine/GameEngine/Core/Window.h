
#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>

namespace DrageEngine
{
    class Window
    {
        public:
            bool IsInitialized() const;
        
            int GetWidth() const;
            int GetHeight() const;
            int GetDrawableWidth() const;
            int GetDrawableHeight() const;
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
            friend class Editor;
        
            Window(int width, int height, bool fullscreen = false, const std::string &caption = "");
            ~Window();

            void SwapBuffers();
        
            static int HandleWindowEvents(void* data, SDL_Event* event);
            void HandleResize();
        
            bool init;
            int flags;
            int width;
            int height;
            int drawableWidth;
            int drawableHeight;
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
