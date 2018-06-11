/************************************************************************/
/* 
@author:    junliang
@brief:     aabb

@help: tangziwen CubeEngine AABB
@time:      2017-8-23
*/
/************************************************************************/
#pragma once

#include "yy_vec3f.h"
#include "yy_mat4f.h"
#include <vector>
#include "math/inc/yy_math.h"
NS_YY_BEGIN

class AABB
{
public:
    AABB();
    ~AABB();
    void update(Vec3f *vec, int num);
    void update(Vec3f vec);
    void transForm(Mat4f mat);
    void reset();
    void merge(AABB box);
    Vec3f Min() const;
    void setMin(const Vec3f &min);

    Vec3f Max() const;

    Vec3f centre();

    std::vector<AABB> split8();
    bool isInside(Vec3f p) const;
    void setMax(const Vec3f &max);

    // °üº¬
    bool isCanCotain(AABB aabb);

    // Ïà½»
    bool isIntersect(AABB aabb, Vec3f &overLap);
private:
    Vec3f m_min;
    Vec3f m_max;
};
NS_YY_END
