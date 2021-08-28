
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
        bool inputForward;
        bool inputBack;
        float inputTurn;
        bool inputBoost;
        bool inputDrift;
    
        // Normal Movement
        Vector3 velocity;
        float acceleration;
        float maxVelocity;
        float maxReverseVelocity;
        float velocityLimitSmoothing;
        float deceleration;
        float brakeDecel;
        bool movingForwards;
    
        // Turning & Drifting
        float normalTurnSpeed;
        float driftTurnSpeed;
        float turnSpeed;
        float rotationSmoothing;
        float grip;
        float normalGrip;
        float driftGrip;
        bool drifting;
    
        // Jumping & Falling
        Terrain *terrain;
        float gravity;
        float minJumpHeight;
        bool grounded;
        float airTurnControlFactor;
        bool jumping;
        float jumpFactor;
    
        // Boost
        bool usingBoost;
        float boost;
        float boostGenRate;
        float minBoostRequired;
        float maxBoostTime;
        float boostMaxVelocity;
        float boostAcceleration;
        float wheelieAngleMax;
        float wheelieAngleInc;
        float wheelieAngle;
    
        // Rendering
        Entity *visualEntity;
        float visualY;
        float visualYSmoothing;
};

#endif
