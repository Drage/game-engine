
#include "Input.h"
#include "Application.h"
#include "Debug.h"
#include "MathUtils.h"

using namespace DrageEngine;

Input::Input()
{
    for (int i = 0; i < Key::KEY_COUNT; i++)
        keyState[i] = false;
    
    for (int i = 0; i < Mouse::BUTTON_COUNT; i++)
        mouseButtonState[i] = false;
    
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    mousePosition = Vector2(mouseX, mouseY);
    
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    int numJoysticks = SDL_NumJoysticks();
    joystickInput.resize(numJoysticks);
    for (int i = 0; i < numJoysticks; i++)
    {
        SDL_Joystick *joystick = SDL_JoystickOpen(0);
        joysticks.push_back(joystick);
    }
    
    SDL_AddEventWatch(HandleInputEvents, NULL);
}

Input::~Input()
{
    for (int i = 0; i < joysticks.size(); i++)
        SDL_JoystickClose(joysticks[i]);
}

void Input::Clear()
{
    keyPresses.clear();
    keyReleases.clear();
    
    mouseButtonPresses.clear();
    mouseButtonReleases.clear();
    mouseDelta = Vector2(0);
    
    for (int i = 0; i < joystickInput.size(); i++)
    {
        joystickInput[i].buttonPresses.clear();
        joystickInput[i].buttonReleases.clear();
    }
}

bool Input::GetKeyDown(Key::Code key) const
{
    for (int i = 0; i < keyPresses.size(); i++)
    {
        if (keyPresses[i] == key)
            return true;
    }
    return false;
}

bool Input::GetKeyUp(Key::Code key) const
{
    for (int i = 0; i < keyReleases.size(); i++)
    {
        if (keyReleases[i] == key)
            return true;
    }
    return false;
}

bool Input::GetKey(Key::Code key) const
{
    return keyState[key];
}

bool Input::GetMouseButtonDown(Mouse::Button button) const
{
    for (int i = 0; i < mouseButtonPresses.size(); i++)
    {
        if (mouseButtonPresses[i] == button)
            return true;
    }
    return false;
}

bool Input::GetMouseButtonUp(Mouse::Button button) const
{
    for (int i = 0; i < mouseButtonReleases.size(); i++)
    {
        if (mouseButtonReleases[i] == button)
            return true;
    }
    return false;
}

bool Input::GetMouseButton(Mouse::Button button) const
{
    return mouseButtonState[button];
}

Vector2 Input::GetMousePosition() const
{
    return mousePosition;
}

Vector2 Input::GetMouseDelta() const
{
    return mouseDelta;
}

bool Input::GetJoystickButtonDown(Joystick::Button button, int index) const
{
    if (joystickInput.size() <= index)
        return false;
    
    for (int i = 0; i < joystickInput[index].buttonPresses.size(); i++)
    {
        if (joystickInput[index].buttonPresses[i] == button)
            return true;
    }
    return false;
}

bool Input::GetJoystickButtonUp(Joystick::Button button, int index) const
{
    if (joystickInput.size() <= index)
        return false;
    
    for (int i = 0; i < joystickInput[index].buttonReleases.size(); i++)
    {
        if (joystickInput[index].buttonReleases[i] == button)
            return true;
    }
    return false;
}

bool Input::GetJoystickButton(Joystick::Button button, int index) const
{
    if (joystickInput.size() <= index)
        return false;
        
    return joystickInput[index].button[button];
}

float Input::GetJoystickTrigger(Joystick::Trigger trigger, int index) const
{
    if (joystickInput.size() <= index)
        return 0;
    
    return joystickInput[index].trigger[trigger];
}

Vector2 Input::GetJoystickAxis(Joystick::Axis axis, int index) const
{
    if (joystickInput.size() <= index)
        return Vector2(0);
    
    return joystickInput[index].axis[axis];
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
        keyState[key] = true;
        keyPresses.push_back(key);
    }
    else if (event->type == SDL_KEYUP)
    {
        Key::Code key = (Key::Code)event->key.keysym.scancode;
        keyState[key] = false;
        keyReleases.push_back(key);
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        Mouse::Button button = (Mouse::Button)event->button.button;
        mouseButtonState[button] = true;
        mouseButtonPresses.push_back(button);
    }
    else if (event->type == SDL_MOUSEBUTTONUP)
    {
        Mouse::Button button = (Mouse::Button)event->button.button;
        mouseButtonState[button] = false;
        mouseButtonReleases.push_back(button);
    }
    else if (event->type == SDL_MOUSEMOTION)
    {
        mousePosition.x = event->motion.x;
        mousePosition.y = event->motion.y;
        mouseDelta.x = event->motion.xrel;
        mouseDelta.y = event->motion.yrel;
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
            case 0: joystickInput[index].axis[Joystick::Axis::LeftStick].x = value; break;
            case 1: joystickInput[index].axis[Joystick::Axis::LeftStick].y = -value; break;
            case 2: joystickInput[index].axis[Joystick::Axis::RightStick].x = value; break;
            case 3: joystickInput[index].axis[Joystick::Axis::RightStick].y = -value; break;
                
            case 4: joystickInput[index].trigger[Joystick::Trigger::LeftTrigger] = value; break;
            case 5: joystickInput[index].trigger[Joystick::Trigger::RightTrigger] = value; break;
        }
    }
    else if (event->type == SDL_JOYBUTTONDOWN)
    {
        if (event->jbutton.button < Joystick::BUTTON_COUNT)
        {
            int joyIndex = event->jbutton.which;
            Joystick::Button button = (Joystick::Button)event->jbutton.button;
            joystickInput[joyIndex].button[button] = true;
            joystickInput[joyIndex].buttonPresses.push_back(button);
        }
    }
    else if (event->type == SDL_JOYBUTTONUP)
    {
        if (event->jbutton.button < Joystick::BUTTON_COUNT)
        {
            int joyIndex = event->jbutton.which;
            Joystick::Button button = (Joystick::Button)event->jbutton.button;
            joystickInput[joyIndex].button[button] = false;
            joystickInput[joyIndex].buttonReleases.push_back(button);
        }
    }
}
