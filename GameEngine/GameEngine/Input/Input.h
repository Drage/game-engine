
#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <vector>
#include "KeyCodes.h"
#include "MouseButtons.h"
#include "Joystick.h"
#include "Vector2.h"

namespace DrageEngine
{
    class Input
    {
        public:
            bool GetKeyDown(Key::Code key) const;
            bool GetKeyUp(Key::Code key) const;
            bool GetKey(Key::Code key) const;
        
            bool GetMouseButtonDown(Mouse::Button button) const;
            bool GetMouseButtonUp(Mouse::Button button) const;
            bool GetMouseButton(Mouse::Button button) const;
            Vector2 GetMousePosition() const;
            Vector2 GetMouseDelta() const;
        
            bool GetJoystickButtonDown(Joystick::Button button, int index = 0) const;
            bool GetJoystickButtonUp(Joystick::Button button, int index = 0) const;
            bool GetJoystickButton(Joystick::Button button, int index = 0) const;
            float GetJoystickTrigger(Joystick::Trigger trigger, int index = 0) const;
            Vector2 GetJoystickAxis(Joystick::Axis axis, int index = 0) const;
            
        private:
            friend class Application;
            Input();
            ~Input();
            void Clear();
        
            static int HandleInputEvents(void* data, SDL_Event* event);
            void CaptureInput(SDL_Event* event);
        
            std::vector<Key::Code> keyPresses;
            std::vector<Key::Code> keyReleases;
            bool keyState[Key::KEY_COUNT];
        
            std::vector<Mouse::Button> mouseButtonPresses;
            std::vector<Mouse::Button> mouseButtonReleases;
            bool mouseButtonState[Mouse::BUTTON_COUNT];
            Vector2 mousePosition;
            Vector2 mouseDelta;
        
            std::vector<SDL_Joystick*> joysticks;
            typedef struct JoystickInput
            {
                Vector2 axis[Joystick::AXIS_COUNT];
                float trigger[Joystick::TRIGGER_COUNT];
                bool button[Joystick::BUTTON_COUNT];
                std::vector<Joystick::Button> buttonPresses;
                std::vector<Joystick::Button> buttonReleases;
            } JoystickInput;
            std::vector<JoystickInput> joystickInput;
        
    };
}

#endif
