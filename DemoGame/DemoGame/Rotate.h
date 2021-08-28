
#ifndef ROTATE_H
#define ROTATE_H

#include <GameEngine.h>

using namespace DrageEngine;

class Rotate : public Component
{
    public:
        void Init(const ParamList &params);
        void Update();
    
    private:
        float m_speed;
        Vector3 m_axis;
};

#endif
