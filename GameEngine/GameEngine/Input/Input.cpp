
#include "Input.h"
#include "Application.h"
#include "Debug.h"
#include "MathUtils.h"

using namespace DrageEngine;

Input::Input()
{
    for (int i = 0; i < Key::KEY_COUNT; i++)
        m_keyState[i] = false;
    
    for (int i = 0; i < Mouse::BUTTON_COUNT; i++)
        m_mouseButtonState[i] = false;
    
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    m_mousePosition = Vector2(mouseX, mouseY);
    
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    int numJoysticks = SDL_NumJoysticks();
    m_joystickInput.resize(numJoysticks);
    for (int i = 0; i < numJoysticks; i++)
    {
        SDL_Joystick *joystick = SDL_JoystickOpen(0);
        m_joysticks.push_back(joystick);
    }
    
    SDL_AddEventWatch(HandleInputEvents, NULL);
}

Input::~Input()
{
    for (int i = 0; i < m_joysticks.size(); i++)
        SDL_JoystickClose(m_joysticks[i]);
}

void Input::Clear()
{
    m_keyPresses.clear();
    m_keyReleases.clear();
    
    m_mouseButtonPresses.clear();
    m_mouseButtonReleases.clear();
    m_mouseDelta = Vector2(0);
    
    for (int i = 0; i < m_joystickInput.size(); i++)
    {
        m_joystickInput[i].buttonPresses.clear();
        m_joystickInput[i].buttonReleases.clear();
    }
}

bool Input::GetKeyDown(Key::Code key) const
{
    for (int i = 0; i < m_keyPresses.size(); i++)
    {
        if (m_keyPresses[i] == key)
            return true;
    }
    return false;
}

bool Input::GetKeyUp(Key::Code key) const
{
    for (int i = 0; i < m_keyReleases.size(); i++)
    {
        if (m_keyReleases[i] == key)
            return true;
    }
    return false;
}

bool Input::GetKey(Key::Code key) const
{
    return m_keyState[key];
}

bool Input::GetMouseButtonDown(Mouse::Button button) const
{
    for (int i = 0; i < m_mouseButtonPresses.size(); i++)
    {
        if (m_mouseButtonPresses[i] == button)
            return true;
    }
    return false;
}

bool Input::GetMouseButtonUp(Mouse::Button button) const
{
    for (int i = 0; i < m_mouseButtonReleases.size(); i++)
    {
        if (m_mouseButtonReleases[i] == button)
            return true;
    }
    return false;
}

bool Input::GetMouseButton(Mouse::Button button) const
{
    return m_mouseButtonState[button];
}

Vector2 Input::GetMousePosition() const
{
    return m_mousePosition;
}

Vector2 Input::GetMouseDelta() const
{
    return m_mouseDelta;
}

bool Input::GetJoystickButtonDown(Joystick::Button button, int index) const
{
    if (m_joystickInput.size() <= index)
        return false;
    
    for (int i = 0; i < m_joystickInput[index].buttonPresses.size(); i++)
    {
        if (m_joystickInput[index].buttonPresses[i] == button)
            return true;
    }
    return false;
}

bool Input::GetJoystickButtonUp(Joystick::Button button, int index) const
{
    if (m_joystickInput.size() <= index)
        return false;
    
    for (int i = 0; i < m_joystickInput[index].buttonReleases.size(); i++)
    {
        if (m_joystickInput[index].buttonReleases[i] == button)
            return true;
    }
    return false;
}

bool Input::GetJoystickButton(Joystick::Button button, int index) const
{
    if (m_joystickInput.size() <= index)
        return false;
        
    return m_joystickInput[index].button[button];
}

float Input::GetJoystickTrigger(Joystick::Trigger trigger, int index) const
{
    if (m_joystickInput.size() <= index)
        return 0;
    
    return m_joystickInput[index].trigger[trigger];
}

Vector2 Input::GetJoystickAxis(Joystick::Axis axis, int index) const
{
    if (m_joystickInput.size() <= index)
        return Vector2(0);
    
    return m_joystickInput[index].axis[axis];
}

int Input::HandleInputEvents(void* data, SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP
        || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP || event->type == SDL_MOUSEMOTION
        || event->type == SDL_JOYBUTTONDOWN || event->type == SDL_JOYBUTTONUP || event->type == SDL_JOYAXISMOTION)
    {
        app->input->CaptureInput(event);
        return 0;
    }
    return 1;
}

void Input::CaptureInput(SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN && event->key.repeat == 0)
    {
        Key::Code key = (Key::Code)event->key.keysym.scancode;
        m_keyState[key] = true;
        m_keyPresses.push_back(key);
    }
    else if (event->type == SDL_KEYUP)
    {
        Key::Code key = (Key::Code)event->key.keysym.scancode;
        m_keyState[key] = false;
        m_keyReleases.push_back(key);
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        Mouse::Button button = (Mouse::Button)event->button.button;
        m_mouseButtonState[button] = true;
        m_mouseButtonPresses.push_back(button);
    }
    else if (event->type == SDL_MOUSEBUTTONUP)
    {
        Mouse::Button button = (Mouse::Button)event->button.button;
        m_mouseButtonState[button] = false;
        m_mouseButtonReleases.push_back(button);
    }
    else if (event->type == SDL_MOUSEMOTION)
    {
        m_mousePosition.x = event->motion.x;
        m_mousePosition.y = event->motion.y;
        m_mouseDelta.x = event->motion.xrel;
        m_mouseDelta.y = event->motion.yrel;
    }
    else if (event->type == SDL_JOYAXISMOTION)
    {
        int index = event->jaxis.which;
        
        float value = Clamp(event->jaxis.value / 32767.0f, -1.0f, 1.0f);
        
        if (event->jaxis.axis == 4 || event->jaxis.axis == 5)
            value = Normalize(value, -1, 1);
        
        if (abs(value) < Joystick::DEADZONE)
            value = 0;
        
        switch (event->jaxis.axis)
        {
            case 0: m_joystickInput[index].axis[Joystick::Axis::LeftStick].x = value; break;
            case 1: m_joystickInput[index].axis[Joystick::Axis::LeftStick].y = -value; break;
            case 2: m_joystickInput[index].axis[Joystick::Axis::RightStick].x = value; break;
            case 3: m_joystickInput[index].axis[Joystick::Axis::RightStick].y = -value; break;
                
            case 4: m_joystickInput[index].trigger[Joystick::Trigger::LeftTrigger] = value; break;
            case 5: m_joystickInput[index].trigger[Joystick::Trigger::RightTrigger] = value; break;
        }
    }
    else if (event->type == SDL_JOYBUTTONDOWN)
    {
        if (event->jbutton.button < Joystick::BUTTON_COUNT)
        {
            int joyIndex = event->jbutton.which;
            Joystick::Button button = (Joystick::Button)event->jbutton.button;
            m_joystickInput[joyIndex].button[button] = true;
            m_joystickInput[joyIndex].buttonPresses.push_back(button);
        }
    }
    else if (event->type == SDL_JOYBUTTONUP)
    {
        if (event->jbutton.button < Joystick::BUTTON_COUNT)
        {
            int joyIndex = event->jbutton.which;
            Joystick::Button button = (Joystick::Button)event->jbutton.button;
            m_joystickInput[joyIndex].button[button] = false;
            m_joystickInput[joyIndex].buttonReleases.push_back(button);
        }
    }
}
