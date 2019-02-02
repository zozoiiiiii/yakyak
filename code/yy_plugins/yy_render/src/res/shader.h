/************************************************************************/
/* 
@author:    junliang
@brief:      �ο�Skeletonanimation�� glsl.h

Ϊ��Ҫ�õ�mrt������Ⱦ:
ͬʱ���һ������Ĳ�ͬ����Ч��



unityshader: pass
һ��shader��Ӧһ��Ч���� �������һ��render texture�ڲ�ͬʱ����Ⱦ��ͬ��Ч����
�ͱ���Ҫ2��shader.
����: unity3d��һ��shader�����ж��pass, �ֱ��Ӧ��ͬ����ȾЧ����


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
