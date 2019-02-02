/************************************************************************/
/* 
@author:    junliang
@brief:     shader
@time:      2017-6-16
*/
/************************************************************************/
#pragma once

#include <vector>
#include "i_res.h"

class IShader : public IRes
{
	YY_VIRTUAL_BEGIN(IShader, IRes);
	YY_END
public:
    virtual void Load(const char* vsh, const char* fsh) = 0;
    virtual void SetUniform1i(char* Varname, int v0) = 0;
    virtual void SetUniform1f(char* Varname, float v0) = 0;
    virtual void SetUniform2f(char* Varname, float v0, float v1) = 0;
    virtual void SetUniform3f(char* Varname, float v0, float v1, float v2) = 0;
    virtual void SetUniform4f(char* Varname, float v0, float v1, float v2, float v3) = 0;
    virtual void SetUniformMat4fv(char* Varname, int count ,float* val) = 0;
    virtual void Begin(void) = 0;
    virtual void End(void) = 0;
    virtual int GetLoc(char* Varname) = 0;

    virtual unsigned int GetProgramHandle() = 0;
};
