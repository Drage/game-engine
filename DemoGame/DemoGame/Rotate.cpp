
#include "Rotate.h"

void Rotate::Init(const ParamList &params)
{
    m_speed = params.Get<float>("speed", 1.0f);
    m_axis = params.Get<Vector3>("axis", Vector3::UP);
}

void Rotate::Update()
{
    Quaternion rotation = Quaternion::FromAxisAngle(m_axis, m_speed * Time::DeltaTime());
    transform->Rotate(rotation);
}
