
#include "Transform.h"

using namespace DrageEngine;

Transform::Transform()
{
    position = Vector3(0);
    rotation = Quaternion::IDENTITY;
    scale = Vector3(1);
}

Transform::Transform(const Vector3 &position, const Quaternion &rotation, const Vector3 &scale)
{
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

Transform::Transform(const Matrix4x4 &matrix)
{
    FromMatrix(matrix);
}

Transform::Transform(ParamList &params)
{
    position = params.Get<Vector3>("position");
    rotation = params.Get<Quaternion>("rotation", Quaternion::IDENTITY);
    scale = params.Get<Vector3>("scale", Vector3::ONE);
}

Transform Transform::operator* (const Transform &other) const
{
    Transform result;
    
    result.position = position + rotation * other.position;
    
    result.scale.x = scale.x * other.scale.x;
    result.scale.y = scale.y * other.scale.y;
    result.scale.z = scale.z * other.scale.z;
    
    result.rotation = rotation * other.rotation;
    
    return result;
}

void Transform::operator*= (const Transform &other)
{
    *this = *this * other;
}

Transform Transform::operator+ (const Vector3 &translation) const
{
    return Transform(this->position + position, rotation, scale);
}

void Transform::operator+= (const Vector3 &translation)
{
    Translate(translation);
}

Transform Transform::operator* (const Quaternion &rotation) const
{
    return Transform(position, this->rotation * rotation, scale);
}

void Transform::operator*= (const Quaternion &rotation)
{
    Rotate(rotation);
}

Transform Transform::operator* (const Vector3 &scale) const
{
    Vector3 resultScale;
    resultScale.x = this->scale.x * scale.x;
    resultScale.y = this->scale.y * scale.y;
    resultScale.z = this->scale.z * scale.z;
    
    return Transform(position, rotation, resultScale);
}

void Transform::operator*= (const Vector3 &scale)
{
    Scale(scale);
}

void Transform::Translate(const Vector3 &translation)
{
    this->position += translation;
}

void Transform::Rotate(const Quaternion &rotation)
{
    this->rotation *= rotation;
}

void Transform::Scale(const Vector3 &scale)
{
    this->scale.x *= scale.x;
    this->scale.y *= scale.y;
    this->scale.z *= scale.z;
}

void Transform::FromMatrix(const Matrix4x4 &matrix)
{
    position = matrix.GetTranslation();
    scale = matrix.GetScale();
    rotation = matrix.GetRotation();
}

Matrix4x4 Transform::ToMatrix() const
{
    Matrix4x4 translation, scale, rotation;
    
    translation.SetTranslation(this->position);
    scale.SetScale(this->scale);
    rotation.SetRotation(this->rotation);
    
    return translation * scale * rotation;
}

ParamList Transform::ToParamList() const
{
    ParamList params;
    params.Set<Vector3>("position", position);
    params.Set<Quaternion>("rotation", rotation);
    params.Set<Vector3>("scale", scale);
    return params;
}

Vector3 Transform::Forward() const
{
    return rotation * Vector3::FORWARD;
}

Vector3 Transform::Back() const
{
    return rotation * Vector3::BACK;
}

Vector3 Transform::Up() const
{
    return rotation * Vector3::UP;
}

Vector3 Transform::Down() const
{
    return rotation * Vector3::DOWN;
}

Vector3 Transform::Left() const
{
    return rotation * Vector3::LEFT;
}

Vector3 Transform::Right() const
{
    return rotation * Vector3::RIGHT;
}

Vector3 Transform::TransformDirection(const Vector3 &direction) const
{
    return rotation * direction;
}

Vector3 Transform::TransformPoint(const Vector3 &point) const
{
    return ToMatrix() * point;
}

Vector3 Transform::InverseTransformDirection(const Vector3 &direction) const
{
    return rotation.Inverse() * direction;
}

Vector3 Transform::InverseTransformPoint(const Vector3 &point) const
{
    return ToMatrix().Inverse() * point;
}
