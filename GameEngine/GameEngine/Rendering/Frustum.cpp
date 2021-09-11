
#include <math.h>
#include "Frustum.h"
#include "MathUtils.h"
#include "Application.h"

using namespace DrageEngine;

void Frustum::Update(const Vector3 &position, const Vector3 &direction, float nearClipDistance, float farClipDistance, float fov)
{
    Vector3 Z = -direction.Normalized();
    Vector3 X = Vector3::Cross(Vector3::UP, Z).Normalized();
    Vector3 Y = Vector3::Cross(Z, X);

    Vector3 nc = position - Z * nearClipDistance;
    Vector3 fc = position - Z * farClipDistance;

    float tang = (float)tan(DegToRad(fov * 0.5));
    float aspectRatio = app->window->GetAspectRatio();
    float nw = nearClipDistance * tang;
    float nh = nw / aspectRatio;
    float fw = farClipDistance * tang;
    float fh = fw / aspectRatio;

    Vector3 ntl = nc + Y * nh - X * nw;
    Vector3 ntr = nc + Y * nh + X * nw;
    Vector3 nbl = nc - Y * nh - X * nw;
    Vector3 nbr = nc - Y * nh + X * nw;

    Vector3 ftl = fc + Y * fh - X * fw;
    Vector3 ftr = fc + Y * fh + X * fw;
    Vector3 fbl = fc - Y * fh - X * fw;
    Vector3 fbr = fc - Y * fh + X * fw;

    planes[0] = ComputePlane(ntr, ntl, ftl); // Top
    planes[1] = ComputePlane(nbl, nbr, fbr); // Bottom
    planes[2] = ComputePlane(ntl, nbl, fbl); // Left
    planes[3] = ComputePlane(nbr, ntr, fbr); // Right
    planes[4] = ComputePlane(ntl, ntr, nbr); // Near
    planes[5] = ComputePlane(ftr, ftl, fbl); // Far
}

Vector4 Frustum::ComputePlane(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3) const
{
    Vector3 aux1 = v1 - v2;
    Vector3 aux2 = v3 - v2;

    Vector3 normal = Vector3::Cross(aux2, aux1);
    normal.Normalize();

    float d = -Vector3::Dot(normal, v2);
    
    return Vector4(normal, d);
}

bool Frustum::CheckSphere(const Vector3 &position, float radius) const
{
    for (int i = 0; i < 6; i++)
    {
        float dist = Vector3::Dot(position, planes[i].xyz()) + planes[i].w + radius;
        if (dist < 0) return false;
    }
    return true;
}

bool Frustum::CheckBounds(const Bounds &bounds, const Matrix4x4 &modelMatrix) const
{
    Vector4 points[] =
    {
        { bounds.min.x, bounds.min.y, bounds.min.z, 1.0f },
        { bounds.max.x, bounds.min.y, bounds.min.z, 1.0f },
        { bounds.max.x, bounds.max.y, bounds.min.z, 1.0f },
        { bounds.min.x, bounds.max.y, bounds.min.z, 1.0f },
        { bounds.min.x, bounds.min.y, bounds.max.z, 1.0f },
        { bounds.max.x, bounds.min.y, bounds.max.z, 1.0f },
        { bounds.max.x, bounds.max.y, bounds.max.z, 1.0f },
        { bounds.min.x, bounds.max.y, bounds.max.z, 1.0f }
    };

    for (int i = 0; i < 8; ++i)
        points[i] = modelMatrix * points[i];

    for (int i = 0; i < 6; i++)
    {
        bool inside = false;

        for (int j = 0; j < 8; j++)
        {
            if (Vector3::Dot(points[j].xyz(), planes[i].xyz()) + planes[i].w > 0)
            {
                inside = true;
                break;
            }
        }

        if (!inside)
            return false;
    }

    return true;
}
