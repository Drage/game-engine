
#ifndef CAMERA3RDPERSON_H
#define CAMERA3RDPERSON_H

#include <GameEngine.h>

using namespace DrageEngine;

class Camera3rdPerson : public Component
{
    public:
        void Init(const ParamList &params);
        void Start();
        void Update();
    
    private:
        std::string targetPath;
        Entity *target;
        float height;
        float distanceBehind;
        float lookAhead;
        float lookYOffset;
        float moveSmoothing;
        float lookSmoothing;
        float lastLookY;
        Vector3 lookAt;
};

#endif
