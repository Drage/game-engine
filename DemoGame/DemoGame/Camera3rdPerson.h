
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
        std::string m_targetPath;
        GameObject *m_target;
        float m_height;
        float m_distanceBehind;
        float m_lookAhead;
        float m_lookYOffset;
        float m_moveSmoothing;
        float m_lookSmoothing;
        float m_lastLookY;
        Vector3 m_lookAt;
};

#endif
