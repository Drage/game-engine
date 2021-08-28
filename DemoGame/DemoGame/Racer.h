
#ifndef RACER_H
#define RACER_H

#include <GameEngine.h>

using namespace DrageEngine;

class Racer : public Component
{
    public:
        void Init(const ParamList &params);
        void Start();
        void Update();
    
    private:
        void UpdateSteering();
        void UpdateMovement();
        void UpdateBoost();
        void UpdateVertical();
        void UpdateSmoothedDrawY();
        void UpdateRotation();
    
        // Input
        bool m_inputForward;
        bool m_inputBack;
        float m_inputTurn;
        bool m_inputBoost;
        bool m_inputDrift;
    
        // Normal Movement
        Vector3 m_velocity;
        float m_acceleration;
        float m_maxVelocity;
        float m_maxReverseVelocity;
        float m_velocityLimitSmoothing;
        float m_deceleration;
        float m_brakeDecel;
        bool m_movingForwards;
    
        // Turning & Drifting
        float m_normalTurnSpeed;
        float m_driftTurnSpeed;
        float m_turnSpeed;
        float m_rotationSmoothing;
        float m_grip;
        float m_normalGrip;
        float m_driftGrip;
        bool m_drifting;
    
        // Jumping & Falling
        Terrain *m_terrain;
        float m_gravity;
        float m_minJumpHeight;
        bool m_grounded;
        float m_airTurnControlFactor;
        bool m_jumping;
        float m_jumpFactor;
    
        // Boost
        bool m_usingBoost;
        float m_boost;
        float m_boostGenRate;
        float m_minBoostRequired;
        float m_maxBoostTime;
        float m_boostMaxVelocity;
        float m_boostAcceleration;
        float m_wheelieAngleMax;
        float m_wheelieAngleInc;
        float m_wheelieAngle;
    
        // Rendering
        GameObject *m_visualGameObject;
        float m_visualY;
        float m_visualYSmoothing;
};

#endif
