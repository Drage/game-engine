
#include "Racer.h"
#include <iostream>

void Racer::Init(const ParamList &params)
{
    // Input
    m_inputForward = false;
    m_inputBack = false;
    m_inputTurn = 0;
    m_inputBoost = false;
    m_inputDrift = false;
    
    // Normal Movement
    m_maxVelocity = 36;
    m_maxReverseVelocity = 16;
    m_velocityLimitSmoothing = 54;
    m_acceleration = 32;
    m_deceleration = 16;
    m_brakeDecel = 32;
    m_movingForwards = true;
    
    // Turning & Drifting
    m_normalTurnSpeed = 1.9f;
    m_driftTurnSpeed = 2.1f;
    m_turnSpeed = m_normalTurnSpeed;
    m_normalGrip = 0.18f;
    m_driftGrip = 0.03f;
    m_grip = m_normalGrip;
    m_drifting = false;
    
    // Jumping & Falling
    m_gravity = 36;
    m_grounded = true;
    m_jumping = false;
    m_airTurnControlFactor = 0.2f;
    m_minJumpHeight = 0.3f;
    m_jumpFactor = 40;
    
    // Boost
    m_usingBoost = false;
    m_boost = 0;
    m_boostGenRate = 0.03f;
    m_minBoostRequired = 0.3f;
    m_maxBoostTime = 2.5f;
    m_boostMaxVelocity = 64;
    m_boostAcceleration = 72;
    m_wheelieAngleMax = 15;
    m_wheelieAngleInc = 3;
    m_wheelieAngle = 0;
    
    // Rendering
    m_visualYSmoothing = 30;
    m_visualY = transform->position.y;
    m_rotationSmoothing = 15;
}

void Racer::Start()
{
    m_terrain = app->GetActiveScene()->Find("Terrain")->GetComponent<Terrain>();
    m_visualGameObject = gameObject->GetChild("Visual");
}

void Racer::Update()
{
    m_inputForward = app->input->GetJoystickButton(Joystick::A) || app->input->GetKey(Key::UP);
    m_inputBack = app->input->GetJoystickButton(Joystick::X) || app->input->GetKey(Key::DOWN);
    m_inputTurn = app->input->GetJoystickAxis(Joystick::LeftStick).x + app->input->GetKey(Key::RIGHT) - app->input->GetKey(Key::LEFT);
    m_inputBoost = app->input->GetJoystickButton(Joystick::B) || app->input->GetKey(Key::B);
    m_inputDrift = app->input->GetJoystickButton(Joystick::RightBumper) || app->input->GetKey(Key::SPACE);

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
    float turnSpeed = m_jumping ? m_turnSpeed * m_airTurnControlFactor : m_turnSpeed;

    // Modify turn speed based on velocity
    float velocityFactor = m_velocity.Magnitude() * 2 / m_maxVelocity;
    velocityFactor = velocityFactor > 1 ? 1 : velocityFactor;
    turnSpeed *= velocityFactor;
    
    // Apply drifting
    if (!m_drifting)
    {
        if (m_inputDrift && m_inputTurn)
        {
            if (m_velocity.Magnitude() > m_maxVelocity * 3/4)
            {
                m_drifting = true;
                m_grip = m_driftGrip;
                m_turnSpeed = m_driftTurnSpeed;
            }
        }
    }
    else
    {
        if (!m_inputDrift || m_velocity.Magnitude() < m_maxVelocity * 3/4 || !m_inputTurn)
        {
            m_drifting = false;
            m_grip = m_normalGrip;
            m_turnSpeed = m_normalTurnSpeed;
        }
    }
    
    // Rotate
    if ((m_inputTurn < 0 && m_movingForwards) || (m_inputTurn > 0 && !m_movingForwards))
    {
        transform->rotation *= Quaternion::FromAxisAngle(Vector3::UP, abs(m_inputTurn) * turnSpeed * Time::DeltaTime());
    }
    if ((m_inputTurn > 0 && m_movingForwards) || (m_inputTurn < 0 && !m_movingForwards))
    {
        transform->rotation *= Quaternion::FromAxisAngle(Vector3::UP, abs(m_inputTurn) * -turnSpeed * Time::DeltaTime());
    }
}

void Racer::UpdateMovement()
{
    Vector3 forward = transform->rotation * Vector3::FORWARD;
    
    if (!m_usingBoost)
    {
        // Apply acceleration/decceleration
        if (m_inputForward)
        {
            m_velocity += forward * m_acceleration * Time::DeltaTime();
        }
        else if (m_inputBack)
        {
            m_velocity -= forward * m_brakeDecel * Time::DeltaTime();
        }
        else
        {
            // Decelerate if no input
            m_velocity -= m_velocity.Normalized() * m_deceleration * Time::DeltaTime();

            if (m_velocity.Magnitude() < m_deceleration * Time::DeltaTime())
                m_velocity = Vector3(0);
        }
        
        // Limit velocity
        if (m_movingForwards)
        {
            if (m_velocity.Magnitude() > m_maxVelocity)
            {
                float velocityMagnitude = m_velocity.Magnitude();
                float maxVelocity = velocityMagnitude + (m_maxVelocity - velocityMagnitude) * m_velocityLimitSmoothing * Time::DeltaTime();
                m_velocity = m_velocity.Normalized() * maxVelocity;
            }
        }
        else
        {
            if (m_velocity.Magnitude() > m_maxReverseVelocity)
                m_velocity = m_velocity.Normalized() * m_maxReverseVelocity;
        }
    }
    else
    {
        // Boost acceleration
        m_velocity += forward * m_boostAcceleration * Time::DeltaTime();
        if (m_velocity.Magnitude() > m_boostMaxVelocity)
            m_velocity = m_velocity.Normalized() * m_boostMaxVelocity;
    }
    
    // Test if moving forwards
    if (Vector3::Angle(m_velocity, forward) < 1.57079633f)
        m_movingForwards = true;
    else
        m_movingForwards = false;
    
    // Alter velocity based on direction
    float velocityMagnitude = m_velocity.Magnitude();
    if (m_movingForwards && Vector3::Angle(m_velocity, forward) > 0.01f)
    {
        m_velocity.x = (m_velocity.x * (1-m_grip)) + (forward.x * m_grip * velocityMagnitude);
        m_velocity.z = (m_velocity.z * (1-m_grip)) + (forward.z * m_grip * velocityMagnitude);
    }
    else if (!m_movingForwards && Vector3::Angle(m_velocity, -forward) > 0.01f)
    {
        m_velocity.x = (m_velocity.x * (1-m_grip)) + (-forward.x * m_grip * velocityMagnitude);
        m_velocity.z = (m_velocity.z * (1-m_grip)) + (-forward.z * m_grip * velocityMagnitude);
    }
    
    // Update position
    transform->position += m_velocity * Time::DeltaTime();
}

void Racer::UpdateBoost()
{
    if (m_inputBoost && m_boost >= m_minBoostRequired)
    {
        m_usingBoost = true;
    }

    if (m_usingBoost)
    {
        m_boost -= Time::DeltaTime() / m_maxBoostTime;

        if (m_boost <= 0)
        {
            m_boost = 0;
            m_usingBoost = false;
        }
    }
    else
    {
        if (m_boost < 1)
            m_boost += m_boostGenRate * Time::DeltaTime();
        else
            m_boost = 1;
    }
}

void Racer::UpdateVertical()
{
    float terrainHeight = m_terrain->HeightAt(transform->position);
    
    m_grounded = false;
    if (transform->position.y <= terrainHeight + 0.05f)
    {
        if (transform->position.y < terrainHeight)
            m_jumping = false;

        m_grounded = true;
        transform->position.y = terrainHeight;

        if (m_velocity.y < 0)
            m_velocity.y = 0;

        Vector3 back = transform->rotation * Vector3::BACK;
        Vector3 last = back * 1.3f + transform->position;
        Vector3 lastLast = back * 2.6f + transform->position;
        float lastY = m_terrain->HeightAt(last);
        float lastLastY = m_terrain->HeightAt(lastLast);

        if (lastY - lastLastY > m_minJumpHeight && !m_jumping)
        {
            // Do jump
            m_velocity.y = (lastY - lastLastY) * m_velocity.Magnitude() / m_maxVelocity * m_jumpFactor;
            m_jumping = true;
        }
    }
    
    // Apply gravity
    m_velocity.y -= m_gravity * Time::DeltaTime();
}

void Racer::UpdateSmoothedDrawY()
{
    float actualY = transform->position.y;

    // Smooth out y position if grounded
    if (m_grounded)
    {
        m_visualY = Lerp(m_visualY, actualY, m_visualYSmoothing * Time::DeltaTime());
        m_visualGameObject->transform.position.y = m_visualY - actualY;
    }
    else
    {
        m_visualY = actualY;
        m_visualGameObject->transform.position.y = 0;
    }
}

void Racer::UpdateRotation()
{
    if (m_grounded)
    {
        // Get rotation to align with terrain
        Vector3 normal = m_terrain->NormalAt(transform->position);
        Vector3 axis = Vector3::Cross(normal, Vector3::UP);
        float angle = -Vector3::Angle(normal, Vector3::UP);

        // Combine y axis rotation with terrain rotation
        Quaternion newRotation = Quaternion::FromAxisAngle(axis, angle) * Quaternion::FromEuler(0, transform->rotation.ToEuler().y, 0);
        transform->rotation = Quaternion::Slerp(transform->rotation, newRotation, m_rotationSmoothing * Time::DeltaTime());
    }
}
