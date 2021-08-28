
#ifndef MOUSEBUTTONS_H
#define MOUSEBUTTONS_H

namespace DrageEngine
{
    class Mouse
    {
        public:
            typedef enum
            {
                UNKNOWN = 0,
                LEFT = 1,
                RIGHT = 2,
                MIDDLE = 3,
            } Button;
        
            static const int BUTTON_COUNT = 5;
    };
}

#endif
