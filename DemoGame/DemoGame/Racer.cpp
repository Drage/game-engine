
#include "Racer.h"
#include <iostream>

void Racer::Init(const ParamList &params)
{
    // Input
    inputForward = false;
    inputBack = false;
    inputTurn = 0;
    inputBoost = false;
    inputDrift = false;
    
    // Normal Movement
    maxVelocity = 36;
    maxReverseVelocity = 16;
    velocityLimitSmoothing = 54;
    acceleration = 32;
    deceleration = 16;
    brakeDecel = 32;
    movingForwards = true;
    
    // Turning & Drifting
    normalTurnSpeed = 1.9f;
    driftTurnSpeed = 2.1f;
    turnSpeed = normalTurnSpeed;
    normalGrip = 0.18f;
    driftGrip = 0.03f;
    grip = normalGrip;
    drifting = false;
    
    // Jumping & Falling
    gravity = 36;
    grounded = true;
    jumping = false;
    airTurnControlFactor = 0.2f;
    minJumpHeight = 0.3f;
    jumpFactor = 40;
    
    // Boost
    usingBoost = false;
    boost = 0;
    boostGenRate = 0.03f;
    minBoostRequired = 0.3f;
    maxBoostTime = 2.5f;
    boostMaxVelocity = 64;
    boostAcceleration = 72;
    wheelieAngleMax = 15;
    wheelieAngleInc = 3;
    wheelieAngle = 0;
    
    // Rendering
    visualYSmoothing = 30;
    visualY = transform->position.y;
    rotationSmoothing = 15;
}

void Racer::Start()
{
    terrain = app->GetActiveScene()->Find("Terrain")->GetComponent<Terrain>();
    visualGameObject = gameObject->GetChild("Visual");
}

void Racer::Update()
{
    inputForward = app->input->GetJoystickButton(Joystick::A) || app->input->GetKey(Key::UP);
    inputBack = app->input->GetJoystickButton(Joystick::X) || app->input->GetKey(Key::DOWN);
    inputTurn = app->input->GetJoystickAxis(Joystick::LeftStick).x + app->input->GetKey(Key::RIGHT) - app->input->GetKey(Key::LEFT);
    inputBoost = app->input->GetJoystickButton(Joystick::B) || app->input->GetKey(Key::B);
    inputDrift = app->input->GetJoystickButton(Joystick::RightBumper) || app->input->GetKey(Key::SPACE);

    UpdateVertical();
    UpdateSteering();
    UpdateMovement();
    UpdateBoost();
    UpdateSmoothedDrawY();
    UpdateRotation();
}

void Racer::UpdateSteering()
{
    // Modify turn speed if jumping
    float turnSpeed = jumping ? this->turnSpeed * airTurnControlFactor : this->turnSpeed;

    // Modify turn speed based on velocity
    float velocityFactor = velocity.Magnitude() * 2 / maxVelocity;
    velocityFactor = velocityFactor > 1 ? 1 : velocityFactor;
    turnSpeed *= velocityFactor;
    
    // Apply drifting
    if (!drifting)
    {
        if (inputDrift && inputTurn)
        {
            if (velocity.Magnitude() > maxVelocity * 3/4)
            {
                drifting = true;
                grip = driftGrip;
                turnSpeed = driftTurnSpeed;
            }
        }
    }
    else
    {
        if (!inputDrift || velocity.Magnitude() < maxVelocity * 3/4 || !inputTurn)
        {
            drifting = false;
            grip = normalGrip;
            turnSpeed = normalTurnSpeed;
        }
    }
    
    // Rotate
    if ((inputTurn < 0 && movingForwards) || (inputTurn > 0 && !movingForwards))
    {
        transform->rotation *= Quaternion::FromAxisAngle(Vector3::UP, abs(inputTurn) * turnSpeed * Time::DeltaTime());
    }
    if ((inputTurn > 0 && movingForwards) || (inputTurn < 0 && !movingForwards))
    {
        transform->rotation *= Quaternion::FromAxisAngle(Vector3::UP, abs(inputTurn) * -turnSpeed * Time::DeltaTime());
    }
}

void Racer::UpdateMovement()
{
    Vector3 forward = transform->rotation * Vector3::FORWARD;
    
    if (!usingBoost)
    {
        // Apply acceleration/decceleration
        if (inputForward)
        {
            velocity += forward * acceleration * Time::DeltaTime();
        }
        else if (inputBack)
        {
            velocity -= forward * brakeDecel * Time::DeltaTime();
        }
        else
        {
            // Decelerate if no input
            velocity -= velocity.Normalized() * deceleration * Time::DeltaTime();

            if (velocity.Magnitude() < deceleration * Time::DeltaTime())
                velocity = Vector3(0);
        }
        
        // Limit velocity
        if (movingForwards)
        {
            if (velocity.Magnitude() > maxVelocity)
            {
                float velocityMagnitude = velocity.Magnitude();
                float maxVelocity = velocityMagnitude + (this->maxVelocity - velocityMagnitude) * velocityLimitSmoothing * Time::DeltaTime();
                velocity = velocity.Normalized() * maxVelocity;
            }
        }
        else
        {
            if (velocity.Magnitude() > maxReverseVelocity)
                velocity = velocity.Normalized() * maxReverseVelocity;
        }
    }
    else
    {
        // Boost acceleration
        velocity += forward * boostAcceleration * Time::DeltaTime();
        if (velocity.Magnitude() > boostMaxVelocity)
            velocity = velocity.Normalized() * boostMaxVelocity;
    }
    
    // Test if moving forwards
    if (Vector3::Angle(velocity, forward) < 1.57079633f)
        movingForwards = true;
    else
        movingForwards = false;
    
    // Alter velocity based on direction
    float velocityMagnitude = velocity.Magnitude();
    if (movingForwards && Vector3::Angle(velocity, forward) > 0.01f)
    {
        velocity.x = (velocity.x * (1-grip)) + (forward.x * grip * velocityMagnitude);
        velocity.z = (velocity.z * (1-grip)) + (forward.z * grip * velocityMagnitude);
    }
    else if (!movingForwards && Vector3::Angle(velocity, -forward) > 0.01f)
    {
        velocity.x = (velocity.x * (1-grip)) + (-forward.x * grip * velocityMagnitude);
        velocity.z = (velocity.z * (1-grip)) + (-forward.z * grip * velocityMagnitude);
    }
    
    // Update position
    transform->position += velocity * Time::DeltaTime();
}

void Racer::UpdateBoost()
{
    if (inputBoost && boost >= minBoostRequired)
    {
        usingBoost = true;
    }

    if (usingBoost)
    {
        boost -= Time::DeltaTime() / maxBoostTime;

        if (boost <= 0)
        {
            boost = 0;
            usingBoost = false;
        }
    }
    else
    {
        if (boost < 1)
            boost += boostGenRate * Time::DeltaTime();
        else
            boost = 1;
    }
}

void Racer::UpdateVertical()
{
    float terrainHeight = terrain->HeightAt(transform->position);
    
    grounded = false;
    if (transform->position.y <= terrainHeight + 0.05f)
    {
        if (transform->position.y < terrainHeight)
            jumping = false;

        grounded = true;
        transform->position.y = terrainHeight;

        if (velocity.y < 0)
            velocity.y = 0;

        Vector3 back = transform->rotation * Vector3::BACK;
        Vector3 last = back * 1.3f + transform->position;
        Vector3 lastLast = back * 2.6f + transform->position;
        float lastY = terrain->HeightAt(last);
        float lastLastY = terrain->HeightAt(lastLast);

        if (lastY - lastLastY > minJumpHeight && !jumping)
        {
            // Do jump
            velocity.y = (lastY - lastLastY) * velocity.Magnitude() / maxVelocity * jumpFactor;
            jumping = true;
        }
    }
    
    // Apply gravity
    velocity.y -= gravity * Time::DeltaTime();
}

void Racer::UpdateSmoothedDrawY()
{
    float actualY = transform->position.y;

    // Smooth out y position if grounded
    if (grounded)
    {
        visualY = Lerp(visualY, actualY, visualYSmoothing * Time::DeltaTime());
        visualGameObject->transform.position.y = visualY - actualY;
    }
    else
    {
        visualY = actualY;
        visualGameObject->transform.position.y = 0;
    }
}

void Racer::UpdateRotation()
{
    if (grounded)
    {
        // Get rotation to align with terrain
        Vector3 normal = terrain->NormalAt(transform->position);
        Vector3 axis = Vector3::Cross(normal, Vector3::UP);
        float angle = -Vector3::Angle(normal, Vector3::UP);

        // Combine y axis rotation with terrain rotation
        Quaternion newRotation = Quaternion::FromAxisAngle(axis, angle) * Quaternion::FromEuler(0, transform->rotation.ToEuler().y, 0);
        transform->rotation = Quaternion::Slerp(transform->rotation, newRotation, rotationSmoothing * Time::DeltaTime());
    }
}
