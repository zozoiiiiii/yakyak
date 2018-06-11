#include "math/inc/yy_aabb.h"
#include <algorithm>
#include <math.h>

NS_YY_BEGIN

AABB::AABB()
{
    reset();
}

AABB::~AABB()
{

}

void AABB::update(Vec3f *vec, int num)
{
    for (size_t i = 0; i < num; i++)
    {
        // Leftmost point.
        if (vec[i].x < m_min.x)
            m_min.x = vec[i].x;

        // Lowest point.
        if (vec[i].y < m_min.y)
            m_min.y = vec[i].y;

        // Farthest point.
        if (vec[i].z < m_min.z)
            m_min.z = vec[i].z;

        // Rightmost point.
        if (vec[i].x > m_max.x)
            m_max.x = vec[i].x;


        // Highest point.
        if (vec[i].y > m_max.y)
            m_max.y = vec[i].y;

        // Nearest point.
        if (vec[i].z > m_max.z)
            m_max.z = vec[i].z;
    }
}

void AABB::update(Vec3f vec)
{
    // Leftmost point.
    if (vec.x < m_min.x)
        m_min.x = vec.x;

    // Lowest point.
    if (vec.y < m_min.y)
        m_min.y = vec.y;

    // Farthest point.
    if (vec.z < m_min.z)
        m_min.z = vec.z;

    // Rightmost point.
    if (vec.x > m_max.x)
        m_max.x = vec.x;


    // Highest point.
    if (vec.y > m_max.y)
        m_max.y = vec.y ;

    // Nearest point.
    if (vec.z > m_max.z)
        m_max.z = vec.z;
}

void AABB::transForm(Mat4f mat)
{

    Vec3f corners[8];
     // Near face, specified counter-clockwise
    // Left-top-front.
    corners[0] = Vec3f(m_min.x, m_max.y, m_max.z);
    // Left-bottom-front.
    corners[1] = Vec3f(m_min.x, m_min.y, m_max.z);
    // Right-bottom-front.
    corners[2] = Vec3f(m_max.x, m_min.y, m_max.z);
    // Right-top-front.
    corners[3] = Vec3f(m_max.x, m_max.y, m_max.z);

    // Far face, specified clockwise
    // Right-top-back.
    corners[4] = Vec3f(m_max.x, m_max.y, m_min.z);
    // Right-bottom-back.
    corners[5] = Vec3f(m_max.x, m_min.y, m_min.z);
    // Left-bottom-back.
    corners[6] = Vec3f(m_min.x, m_min.y, m_min.z);
    // Left-top-back.
    corners[7] = Vec3f(m_min.x, m_max.y, m_min.z);
    for(int i =0 ; i < 8 ; i++)
    {
        Vec4f result = mat*Vec4f(corners[i].x,corners[i].y,corners[i].z,1);
        corners[i] = Vec3f(result.x,result.y,result.z);
    }
    reset();
    update(corners,8);
}

void AABB::reset()
{
    //m_min = Vec3f(999,999,999);
    //m_max = Vec3f(-999,-999,-999);

    m_min = Vec3f(0,0,0);
    m_max = Vec3f(0,0,0);
}

void AABB::merge(AABB box)
{
        // Calculate the new minimum point.
        m_min.x = std::min(m_min.x, box.Min().x);
        m_min.y = std::min(m_min.y, box.Min().y);
        m_min.z = std::min(m_min.z, box.Min().z);

        // Calculate the new maximum point.
        m_max.x = std::max(m_max.x, box.Max().x);
        m_max.y = std::max(m_max.y, box.Max().y);
        m_max.z = std::max(m_max.z, box.Max().z);
}

Vec3f AABB::Min() const
{
    return m_min;
}

void AABB::setMin(const Vec3f &min)
{
    m_min = min;
}
Vec3f AABB::Max() const
{
    return m_max;
}

Vec3f AABB::centre()
{
    return (m_max + m_min)/2;
}

/**
 * @brief 将当前AABB八等分，返回八个子AABB
 * @note 可以用来构建八叉树
 * @return 八个子AABB
 */
std::vector<AABB> AABB::split8()
{
    AABB aabb_list[8];
    std::vector<AABB> result;
    Vec3f the_centre = centre();

    //bottom
    //#1
    aabb_list[0].update(m_min);
    aabb_list[0].update(the_centre);
    //#2
    aabb_list[1].update(Vec3f(the_centre.x,m_min.y,m_min.z));
    aabb_list[1].update(Vec3f(m_max.x,the_centre.y,the_centre.z));
    //#3
    aabb_list[2].update(Vec3f(the_centre.x,m_min.y,the_centre.z));
    aabb_list[2].update(Vec3f(m_max.x,the_centre.y,m_max.z));
    //#4
    aabb_list[3].update(Vec3f(m_min.x,m_min.y,the_centre.z));
    aabb_list[3].update(Vec3f(the_centre.x,the_centre.y,m_max.z));

    //top
    //#5
    aabb_list[4].update(Vec3f(m_min.x,the_centre.y,m_min.z));
    aabb_list[4].update(Vec3f(the_centre.x,m_max.y,the_centre.z));
    //#6
    aabb_list[5].update(Vec3f(the_centre.x,the_centre.y,m_min.z));
    aabb_list[5].update(Vec3f(m_max.x,m_max.y,the_centre.z));
    //#7
    aabb_list[6].update(Vec3f(the_centre.x,the_centre.y,the_centre.z));
    aabb_list[6].update(Vec3f(m_max.x,m_max.y,m_max.z));
    //#8
    aabb_list[7].update(Vec3f(m_min.x,the_centre.y,the_centre.z));
    aabb_list[7].update(Vec3f(the_centre.x,m_max.y,m_max.z));

    for(int i =0;i<8;i++)
    {
        result.push_back(aabb_list[i]);
    }
    return result;
}

bool AABB::isInside(Vec3f p) const
{
    if(p.x>=m_min.x&&p.y>=m_min.y && p.z>= m_min.z
            &&p.x<=m_max.x && p.y <=m_max.y && p.z <= m_max.z)
    {
        return true;
    }
    else{
        return false;
    }
}

void AABB::setMax(const Vec3f &max)
{
    m_max = max;
}

bool AABB::isCanCotain(AABB aabb)
{
    if(m_min.x <aabb.Min().x && m_min.y <aabb.Min().y && m_min.z <aabb.Min().z
            && m_max.x>aabb.Max().x && m_max.y>aabb.Max().y && m_max.z>aabb.Max().z)
    {
        return true;
    }else
    {
        return false;
    }
}

bool AABB::isIntersect(AABB aabb,Vec3f &overLap)
{
    bool isHit= ((m_min.x >= aabb.m_min.x && m_min.x <= aabb.m_max.x) || (aabb.m_min.x >= m_min.x && aabb.m_min.x <= m_max.x)) &&
           ((m_min.y >= aabb.m_min.y && m_min.y <= aabb.m_max.y) || (aabb.m_min.y >= m_min.y && aabb.m_min.y <= m_max.y)) &&
           ((m_min.z >= aabb.m_min.z && m_min.z <= aabb.m_max.z) || (aabb.m_min.z >= m_min.z && aabb.m_min.z <= m_max.z));
    if(isHit)
    {
        if(Min().x<aabb.Min().x || Max().x >aabb.Max().x)
        {
            float offset_x_1 =aabb.Max().x - Min().x,offset_x_2 = aabb.Min().x - Max().x;
            if(fabs(offset_x_1)< fabs(offset_x_2))
            {
                overLap.x = offset_x_1;
            }else{
                overLap.x = offset_x_2;
            }
        }
        if(Min().y<aabb.Min().y || Max().y >aabb.Max().y)
        {
            float offset_y_1 =aabb.Max().y - Min().y,offset_y_2 = aabb.Min().y - Max().y;

            if(fabs(offset_y_1)< fabs(offset_y_2))
            {
                overLap.y = offset_y_1;
            }else{
                overLap.y = offset_y_2;
            }

        }
        if(Min().z<aabb.Min().z || Max().z >aabb.Max().z)
        {
            float offset_z_1 =aabb.Max().z - Min().z,offset_z_2 = aabb.Min().z - Max().z;

            if(fabs(offset_z_1)< fabs(offset_z_2))
            {
                overLap.z = offset_z_1;
            }else{
                overLap.z = offset_z_2;
            }
        }

    }
    return isHit;
}

NS_YY_END









