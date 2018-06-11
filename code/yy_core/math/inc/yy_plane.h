/************************************************************************/
/* 
@author:    junliang
@brief:     plane
@time:      2017-8-23
*/
/************************************************************************/
#pragma once

#include "yy_vec3f.h"

NS_YY_BEGIN

enum PointSide
{
    IN_PLANE,
    FRONT_PLANE,
    BEHIND_PLANE,
};

class Plane
{
public:
    /**
    * create plane from tree point.
    */
    Plane(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3);

    /**
    * create plane from normal and dist.
    */
    Plane(const Vec3f& normal, float dist);

    /**
    * create plane from normal and a point on plane.
    */
    Plane(const Vec3f& normal, const Vec3f& point);

    /**
     * create a default plan whose normal is (0, 0, 1), and _dist is 0, xoy plan in fact.
     */
    Plane();

    /**
    * init plane from tree point.
    */
    void initPlane(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3);

    /**
    * init plane from normal and dist.
    */
    void initPlane(const Vec3f& normal, float dist);

    /**
    * init plane from normal and a point on plane.
    */
    void initPlane(const Vec3f& normal, const Vec3f& point);

    /**
    * dist to plane, > 0 normal direction
    */
    float dist2Plane(const Vec3f& p) const;

    /**
    * Gets the plane's normal.
    */
    const Vec3f& getNormal() const { return _normal; }

    /**
    * Gets the plane's distance to the origin along its normal.
    */
    float getDist() const  { return _dist; }

    /**
    * Return the side where the point is.
    */
    PointSide getSide(const Vec3f& point) const;

    Vec3f projectTo(const Vec3f& point) const;

    bool isFrontFacingTo(const Vec3f& direction) const;

protected:
    Vec3f _normal;
    float _dist;
};

NS_YY_END
