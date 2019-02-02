/************************************************************************/
/* 
@author:    junliang
@brief:      参考Skeletonanimation的 glsl.h

为何要用到mrt多重渲染:
同时获得一个画面的不同后期效果



unityshader: pass
一个shader对应一种效果； 如果想让一个render texture在不同时刻渲染不同的效果，
就必须要2个shader.
综上: unity3d中一个shader可以有多个pass, 分别对应不同的渲染效果。


@time:      2017-10-27
*/
/************************************************************************/
#pragma once

#include <vector>
#include "yy_render/inc/res/i_Shader.h"

class Shader : public IShader
{
    YY_BEGIN(Shader, IShader);
    YY_END
public:
    Shader();
    virtual void Load(const char* vsh, const char* fsh);
    void SetUniform1i(char* Varname, int v0);
    void SetUniform1f(char* Varname, float v0);
    void SetUniform2f(char* Varname, float v0, float v1);
    void SetUniform3f(char* Varname, float v0, float v1, float v2);
    void SetUniform4f(char* Varname, float v0, float v1, float v2, float v3);
    void SetUniformMat4fv(char* Varname, int count ,float* val);
    void Begin(void);
    void End(void);
    int GetLoc(char* Varname);

    unsigned int GetProgramHandle(){return m_program_handle;}
private:
    void Attach(int type,const char* filename);
    void Link();
private:
    std::vector<unsigned int> m_handles;
    unsigned int m_program_handle;
};
