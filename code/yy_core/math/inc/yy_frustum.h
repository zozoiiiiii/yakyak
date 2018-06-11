/************************************************************************/
/* 
@author:    junliang
@brief:     frustum
@time:      2017-8-23
*/
/************************************************************************/
#pragma once


#include "yy_aabb.h"
#include "yy_plane.h"

NS_YY_BEGIN

class Frustum
{
    friend class Camera;
public:
    /**
     * Constructor & Destructor.
     */
    Frustum(): _initialized(false), _clipZ(true){}
    ~Frustum(){}


    /**
     * init frustum from camera.
     */
    bool initFrustumFromProjectMatrix(Mat4f matrix);
    /**
     * is aabb out of frustum.
     */
    bool isOutOfFrustum(const AABB& aabb) const;

    /**
     * get & set z clip. if bclipZ == true use near and far plane
     */
    void setClipZ(bool clipZ) { _clipZ = clipZ; }
    bool isClipZ() { return _clipZ; }

protected:
    /**
     * create clip plane
     */
    void createPlane(Mat4f matrix);

    Plane _plane[6];             // clip plane, left, right, top, bottom, near, far
    bool _clipZ;                // use near and far clip plane
    bool _initialized;
};

NS_YY_END
