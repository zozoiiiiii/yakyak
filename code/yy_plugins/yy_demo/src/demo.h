/************************************************************************/
/* 
@author:    junliang
@brief:     �����ͼ��(computer graphics)

Э��:


1, ʹ����������ϵ��y�����ϣ�

d3d����������ϵ��opengl����������ϵ�� 3dmax����������ϵ��z������
�Կ��豸����������ϵ����z��ָ����Ļ���棬zԽ���ʾ��������ԽԶ��opengl������ϵ�ڽ���ü��ռ��ʱ���ת������ϵ��
------�˴�����������ϵrh�� 3dmax��������ʱͨ������SetCoordSystem(IGameConversionManager::IGAME_OGL)��

1.1, ���������(ͶӰ����)�� y�����ϵ������
��������ϵ��z��ָ����Ļ�⣬ �����ָ��z�Ḻ����
��������ϵ��z��ָ����Ļ�ڣ� �����ָ��z��������


2���ü��ռ�zȡֵ��Χ�� d3d�Ǵ�[0,1]�� opengl�Ǵ�[-1,1]

rt : render target, ��ȾĿ�꣬������frame buffer, ������texture buffer


left handed coordinate system
    +y
    |
    |
    |   /+z
    |  /
    | /
    |/
    -+----------- +x
   /|
  /
eye



right handed coordinate system
            +y
            |
            |
            |
            |
            |
            |
            -+----------- +x
           /|
          /
         /
        /+z
       eye

pitch(rotate x axis):  +y -> +z
yaw(rotate y axis):    +z -> +x
roll(rotate z axis):   +y -> +x



�����ĵ���
http://learnopengl-cn.readthedocs.io/zh/latest/04%20Advanced%20OpenGL/02%20Stencil%20testing/

md5����������
http://www.3dgep.com/loading-and-Animating-md5-Models-with-opengl/

�������ߣ�
http://www.katsbits.com/tools/
http://www.voidcn.com/blog/HoneyCat/cata/1392334/

����ͼ��:
http://www.codeproject.com/Articles/199525/Drawing-nearly-perfect-2D-line-segments-in-OpenGL
http://www.codeproject.com/Articles/226569/Drawing-polylines-by-tessellation

��Ϸ����������
http://flipcode.com/

opengl�̳�
http://ogldev.atspace.co.uk/www/tutorial23/tutorial23.html



http://www.cnblogs.com/crazii/p/4054898.html
http://wenku.baidu.com/link?url=JM02nFe9PiUcYf7RDkX_XVul2QDA1solHM1Unc8eZRvROna712J0FbLnyFzuGFwGM9z0rFUI2A8ldsy6G9k5DtnwFKOl412hZrxqeQjRQTe


@time:      5/12/2016
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include <map>
#include <vector>
#include "yy_render/inc/i_render.h"

#ifdef _WIN32
//#include <Windows.h>

#elif __ANDROID__

#elif __APPLE__
#endif

class Demo : public YY::BaseObject
{
    YY_BEGIN(Demo, BaseObject);
    YY_END

public:
    virtual ~Demo(){}
    virtual void OnCreate(const VariantMap& args);
    virtual void OnDestroy();
    virtual void OnExcute(float sec);
};


