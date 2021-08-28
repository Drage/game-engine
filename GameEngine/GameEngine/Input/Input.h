
#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
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
        
            std::vector<Key::Code> m_keyPresses;
            std::vector<Key::Code> m_keyReleases;
            bool m_keyState[Key::KEY_COUNT];
        
            std::vector<Mouse::Button> m_mouseButtonPresses;
            std::vector<Mouse::Button> m_mouseButtonReleases;
            bool m_mouseButtonState[Mouse::BUTTON_COUNT];
            Vector2 m_mousePosition;
            Vector2 m_mouseDelta;
        
            std::vector<SDL_Joystick*> m_joysticks;
            typedef struct JoystickInput
            {
                Vector2 axis[Joystick::AXIS_COUNT];
                float trigger[Joystick::TRIGGER_COUNT];
                bool button[Joystick::BUTTON_COUNT];
                std::vector<Joystick::Button> buttonPresses;
                std::vector<Joystick::Button> buttonReleases;
            } JoystickInput;
            std::vector<JoystickInput> m_joystickInput;
        
    };
}

#endif
