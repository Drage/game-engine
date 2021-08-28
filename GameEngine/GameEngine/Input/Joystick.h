
#ifndef JOYSTICK_H
#define JOYSTICK_H

namespace DrageEngine
{
    class Joystick
    {
        public:
            typedef enum
            {
                A,
                B,
                X,
                Y,
                Select,
                Home,
                Start,
                LeftStickClick,
                RightStickClick,
                LeftBumper,
                RightBumper,
                DPadUp,
                DPadDown,
                DPadLeft,
                DPadRight
            } Button;
            static const int BUTTON_COUNT = 15;
        
            typedef enum
            {
                LeftTrigger,
                RightTrigger
            } Trigger;
            static const int TRIGGER_COUNT = 2;
        
            typedef enum
            {
                LeftStick,
                RightStick
            } Axis;
            static const int AXIS_COUNT = 2;
        
            static constexpr float DEADZONE = 0.1f;
    };
}

#endif
