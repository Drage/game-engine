
#include "Rotate.h"

void Rotate::Init(const ParamList &params)
{
    speed = params.Get<float>("speed", 1.0f);
    axis = params.Get<Vector3>("axis", Vector3::UP);
}

void Rotate::Update()
{
    Quaternion rotation = Quaternion::FromAxisAngle(axis, speed * Time::DeltaTime());
    transform->Rotate(rotation);
}
