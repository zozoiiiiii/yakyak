/************************************************************************/
/* 
@author:    junliang
@brief:     计算机图形(computer graphics)

协议:


1, 使用右手坐标系，y轴向上；

d3d是左手坐标系，opengl是右手坐标系， 3dmax是右手坐标系，z轴向上
显卡设备是左手坐标系，即z轴指向屏幕里面，z越大表示距离视线越远，opengl的右手系在进入裁剪空间的时候会转换左手系。
------此处用右手坐标系rh， 3dmax导出数据时通过设置SetCoordSystem(IGameConversionManager::IGAME_OGL)来

1.1, 摄像机方向(投影方向)， y轴向上的情况下
右手坐标系的z轴指向屏幕外， 摄像机指向z轴负方向
左手坐标系的z轴指向屏幕内； 摄像机指向z轴正方向


2，裁剪空间z取值范围， d3d是从[0,1]， opengl是从[-1,1]

rt : render target, 渲染目标，可以是frame buffer, 可以是texture buffer


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



帮助文档：
http://learnopengl-cn.readthedocs.io/zh/latest/04%20Advanced%20OpenGL/02%20Stencil%20testing/

md5骨骼动画：
http://www.3dgep.com/loading-and-Animating-md5-Models-with-opengl/

导出工具：
http://www.katsbits.com/tools/
http://www.voidcn.com/blog/HoneyCat/cata/1392334/

几何图形:
http://www.codeproject.com/Articles/199525/Drawing-nearly-perfect-2D-line-segments-in-OpenGL
http://www.codeproject.com/Articles/226569/Drawing-polylines-by-tessellation

游戏开发帮助：
http://flipcode.com/

opengl教程
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


