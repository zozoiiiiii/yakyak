#include "math/inc/yy_plane.h"


NS_YY_BEGIN

Plane::Plane()
:
_normal(0.f, 0.f, 1.f),
_dist(0.f)
{

}

// create plane from tree point
Plane::Plane(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3)
{
    initPlane(p1, p2, p3);
}

// create plane from normal and dist
Plane::Plane(const Vec3f &normal, float dist)
{
    initPlane(normal, dist);
}

// create plane from normal and a point on plane
Plane::Plane(const Vec3f &normal, const Vec3f &point)
{
    initPlane(normal, point);
}

void Plane::initPlane(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3)
{
    Vec3f p21 = p2 - p1;
    Vec3f p32 = p3 - p2;
    _normal = p21.cross(p32);
    _normal.norm();
    _dist = _normal.dot(p1);
}

void Plane::initPlane(const Vec3f &normal, float dist)
{
    float oneOverLength = 1 / normal.len();
    _normal = normal * oneOverLength;
    _dist = dist * oneOverLength;
}

void Plane::initPlane(const Vec3f &normal, const Vec3f &point)
{
    _normal = normal;
    _normal.norm();
    _dist =  _normal.dot(point);
}

float Plane::dist2Plane(const Vec3f &p) const
{
    return _normal.dot(p) - _dist;
}


PointSide Plane::getSide(const Vec3f &point) const
{
    float dist = dist2Plane(point);
    if (dist > 0)
        return FRONT_PLANE;
    else if (dist < 0)
        return BEHIND_PLANE;
    else
        return IN_PLANE;
}

Vec3f Plane::projectTo(const Vec3f &point) const
{
    float dist = dist2Plane(point);
    return point - _normal * dist;
}

bool Plane::isFrontFacingTo(const Vec3f &direction) const
{
    double dot = _normal.dot(direction);
    return (dot <= 0);
}



NS_YY_END