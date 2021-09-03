
#ifndef CLOUD_H
#define CLOUD_H

#include <GameEngine.h>

using namespace DrageEngine;

class Cloud : public Component
{
    public:
        void Init(const ParamList &params);
        void Update();
        ~Cloud();
    
    private:
        float size;
        float speed;
        float yPosition;
        float orbitRadius;
        float angle;
        Renderable *renderable;
};

#endif
