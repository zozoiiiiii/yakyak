#include "math/inc/yy_frustum.h"
#include "math/inc/yy_mat4f.h"

NS_YY_BEGIN


bool Frustum::initFrustumFromProjectMatrix(Mat4f matrix)
{
    _initialized = true;
    createPlane(matrix);
    return true;
}

bool Frustum::isOutOfFrustum(const AABB& aabb) const
{
    if (_initialized)
    {
        Vec3f point;

        int plane = _clipZ ? 6 : 4;
        for (int i = 0; i < plane; i++)
        {
            const Vec3f& normal = _plane[i].getNormal();
            point.x = normal.x < 0 ? aabb.Max().x: aabb.Min ().x;
            point.y = normal.y < 0 ? aabb.Max().y: aabb.Min ().y;
            point.z = normal.z < 0 ? aabb.Max().z: aabb.Min ().z;

            if (_plane[i].getSide(point) == PointSide::FRONT_PLANE )
                return true;
        }
    }
    return false;
}


void Frustum::createPlane(Mat4f matrix)
{
    //ref http://www.lighthouse3d.com/tutorials/view-frustum-culling/clip-space-approach-extracting-the-planes/
    //extract frustum plane
    float * m =matrix.m_data;
   _plane[0].initPlane(-Vec3f(m[3] + m[0], m[7] + m[4], m[11] + m[8]), (m[15] + m[12]));//left
   _plane[1].initPlane(-Vec3f(m[3] - m[0], m[7] - m[4], m[11] - m[8]), (m[15] - m[12]));//right
   _plane[2].initPlane(-Vec3f(m[3] + m[1], m[7] + m[5], m[11] + m[9]), (m[15] + m[13]));//bottom
   _plane[3].initPlane(-Vec3f(m[3] - m[1], m[7] - m[5], m[11] - m[9]), (m[15] - m[13]));//top
   _plane[4].initPlane(-Vec3f(m[3] + m[2], m[7] + m[6], m[11] + m[10]), (m[15] + m[14]));//near
   _plane[5].initPlane(-Vec3f(m[3] - m[2], m[7] - m[6], m[11] - m[10]), (m[15] - m[14]));//far
}


NS_YY_END