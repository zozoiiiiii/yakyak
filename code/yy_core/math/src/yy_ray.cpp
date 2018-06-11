#include "math/inc/yy_ray.h"

#define EPSILON 0.000001
NS_YY_BEGIN

Ray::Ray()
{

}

Ray::Ray(Vec3f origin, Vec3f direction)
{
    m_origin = origin;
    m_direction = direction;
}

Ray::~Ray()
{

}
Vec3f Ray::origin() const
{
    return m_origin;
}

void Ray::setOrigin(const Vec3f &origin)
{
    m_origin = origin;
}
Vec3f Ray::direction() const
{
    return m_direction;
}

void Ray::setDirection(const Vec3f &direction)
{
    m_direction = direction;
}

Vec3f Ray::intersectPlane(Plane p)
{
    Vec3f point = p .getNormal ()*p.getDist ();

    Vec3f pNormal = p.getNormal();
    float r = pNormal.dot(point - m_origin) / pNormal.dot(m_direction);
    Vec3f result = m_origin + m_direction*r;
    return result;
}

bool Ray::intersectAABB(AABB aabb, RayAABBSide *side, Vec3f &hitPoint)const
{
    Vec3f ptOnPlane;
    Vec3f min = aabb.Min();
    Vec3f max = aabb.Max();

    const Vec3f& origin = m_origin;
    const Vec3f& dir = m_direction;

    float t;

	// 射线的原点如果在包围盒内，则必然与射线相交
      if(origin.x>min.x && origin.y>min.y && origin.z>min.z
              && origin.x<max.x && origin.y<max.y && origin.z < max.z)
      {
          return true;
      }

    if (dir.x != 0.f)
    {
        if (dir.x > 0)
            t = (min.x - origin.x) / dir.x;
        else
            t = (max.x - origin.x) / dir.x;

        if (t > 0.f)
        {
            ptOnPlane = origin +  dir * t;

            if (min.y < ptOnPlane.y && ptOnPlane.y < max.y && min.z < ptOnPlane.z && ptOnPlane.z < max.z)
            {
                hitPoint = ptOnPlane;
                if(side)
                {
                    if(ptOnPlane.x<max.x)
                        (*side) = RayAABBSide::left;
                    else
                        (*side) = RayAABBSide::right;
                }
                return true;
            }
        }
    }

    if (dir.y != 0.f)
    {
        if (dir.y > 0)
            t = (min.y - origin.y) / dir.y;
        else
            t = (max.y - origin.y) / dir.y;

        if (t > 0.f)
        {
            ptOnPlane = origin +  dir * t;

            if (min.z < ptOnPlane.z && ptOnPlane.z < max.z && min.x < ptOnPlane.x && ptOnPlane.x < max.x)
            {
                hitPoint = ptOnPlane;
                if(side)
                {
                    if(ptOnPlane.y<max.y)
                        (*side) = RayAABBSide::down;
                    else
                        (*side) = RayAABBSide::up;
                }
                return true;
            }
        }
    }

    if (dir.z != 0.f)
    {
        if (dir.z > 0)
            t = (min.z - origin.z) / dir.z;
        else
            t = (max.z - origin.z) / dir.z;

        if (t > 0.f)
        {
            ptOnPlane = origin +  dir * t;

            if (min.x < ptOnPlane.x && ptOnPlane.x < max.x && min.y < ptOnPlane.y && ptOnPlane.y < max.y)
            {
                hitPoint = ptOnPlane;
                if(side)
                {
                    if(ptOnPlane.z<max.z)
                        (*side) = RayAABBSide::back;
                    else
                        (*side) = RayAABBSide::front;
                }
                return true;
            }
        }
    }
    return false;
}

bool Ray::intersectTriangle(const Vec3f &v1, const Vec3f &v2, const Vec3f &v3, float *out) const
{
    Vec3f D = m_direction;
    Vec3f e1, e2;  //Edge1, Edge2
    Vec3f P, Q, T;
    float det, inv_det, u, v;
    float t;

    //Find vectors for two edges sharing V1
    e1 = v2 - v1;
    e2 = v3 - v1;
    //Begin calculating determinant - also used to calculate u parameter
    P = m_direction.cross(e2);
    //if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
    det = e1.dot(P);
    //NOT CULLING
    if(det > -EPSILON && det < EPSILON) return 0;
    inv_det = 1.f / det;

    //calculate distance from V1 to ray origin
    T = m_origin - v1;

    //Calculate u parameter and test bound
    u = T.dot(P) * inv_det;
    //The intersection lies outside of the triangle
    if(u < 0.f || u > 1.f) return 0;

    //Prepare to test v parameter
    Q = T.cross(e1);

    //Calculate V parameter and test bound
    v = D.dot(Q) * inv_det;
    //The intersection lies outside of the triangle
    if(v < 0.f || u + v  > 1.f) return 0;

    t = e2.dot(Q) * inv_det;

    if(t > EPSILON) { //ray intersection
        if (out) *out = t;
        return true;
    }

    // No hit, no win
    return false;
}

} // namespace tzw

