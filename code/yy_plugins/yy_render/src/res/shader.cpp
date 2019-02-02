#include "shader.h"
#include "../platform.h"
#include <string>
#include <iostream>
#include <fstream>
#include "yy_core.h"
#include "core/inc/yy_file.h"

#define ogl_check_error() { \
    GLenum error = glGetError();\
    throw_assert( error == GL_NO_ERROR,"OpenGL Error: "<<(int)error<<"\n");}

#ifdef _WIN32

#elif __ANDROID__

FILE* android_fopen(const char* fname, const char* mode);
#define fopen(name, mode) android_fopen(name, mode)

#elif __APPLE__
#endif

char* LoadFileData(const char* fname)
{
    throw_assert(fname!=NULL, "param is NULL.");

    FILE * fp = fopen(fname, "rb");
    throw_assert(fp, "open file failed."<< fname);

    fseek(fp, 0L, SEEK_END);
    int fsize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    char* mem=(char*)malloc(fsize+1);
    for(int i=0;i<fsize+1;i++)mem[i]=0;
    fread (mem, 1, fsize, fp);
    fclose (fp);
    return mem;
}

Shader::Shader()
{
    m_program_handle = 0;
}

void Shader::Load(const char* vsh, const char* fsh)
{
    Attach(GL_VERTEX_SHADER,vsh);
    Attach(GL_FRAGMENT_SHADER,fsh);
    Link();
};

void Shader::Attach(int type,const char* filename)
{
    char* mem=LoadFileData(filename);
    GLuint handle = glCreateShader(type);
    glShaderSource(handle, 1, (const char**)(&mem), 0);
    ogl_check_error();
    glCompileShader(handle);
    ogl_check_error();
    free(mem);

    int compileSuccess=0;
    char compilerSpew[256]={0};

    glGetShaderiv(handle, GL_COMPILE_STATUS, &compileSuccess);
    ogl_check_error();
    throw_assert(compileSuccess, "Shader:"<<filename<<" compilerSpew:" << compilerSpew);
    m_handles.push_back(handle);
    //free(mem);
}
void Shader::Link()
{
    m_program_handle = glCreateProgram();
    for (int i=0;i<m_handles.size();i++)
    {
        glAttachShader(m_program_handle, m_handles[i]);
        ogl_check_error();
    }

    glLinkProgram(m_program_handle);
    ogl_check_error();

    int linkSuccess;
    char compilerSpew[256];
    glGetProgramiv(m_program_handle, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess)
	{
		int maxLength;
		int length;
		glGetProgramiv(m_program_handle, GL_INFO_LOG_LENGTH, &maxLength);
		char* log = new char[maxLength];
		glGetProgramInfoLog(m_program_handle, maxLength, &length, log);
	}

    throw_assert(linkSuccess, "Shader Linker:\n"<<compilerSpew<<"\nlinkSuccess="<<linkSuccess);
    ogl_check_error();
}

void Shader::SetUniform1i(char* Varname, int v0)
{ 
    int loc = glGetUniformLocation(m_program_handle,Varname);
    if(loc==-1)
        return;

    glUniform1i(loc, v0);

    ogl_check_error();
}
void Shader::SetUniform1f(char* Varname, float v0)
{ 
    int loc = glGetUniformLocation(m_program_handle,Varname);
    if(loc==-1)
        return;

    glUniform1f(loc, v0);
    ogl_check_error();
}
void Shader::SetUniform2f(char* Varname, float v0, float v1)
{ 
    int loc = glGetUniformLocation(m_program_handle,Varname);
    if (loc==-1)
        return;

    glUniform2f(loc, v0, v1);
    ogl_check_error();
}
void Shader::SetUniform3f(char* Varname, float v0, float v1, float v2)
{ 
    int loc = glGetUniformLocation(m_program_handle,Varname);
    if (loc==-1)
        return;

    glUniform3f(loc, v0, v1, v2);
    ogl_check_error();
}
void Shader::SetUniform4f(char* Varname, float v0, float v1, float v2, float v3)
{ 
    int loc = glGetUniformLocation(m_program_handle,Varname);
    if(loc==-1)
        return;

    glUniform4f(loc, v0, v1, v2, v3);
    ogl_check_error();
}
void Shader::SetUniformMat4fv(char* Varname, int count ,float* val)
{ 
    int loc = glGetUniformLocation(m_program_handle,Varname);
    if(loc==-1)
        return;

    glUniformMatrix4fv(loc, count, GL_FALSE, val); 

    ogl_check_error();
}

void Shader::Begin(void)
{
    glUseProgram(m_program_handle);
    ogl_check_error();
}
void Shader::End(void)
{
    glUseProgram(0);
    ogl_check_error();
}
int Shader::GetLoc(char* Varname)
{
    return glGetUniformLocation(m_program_handle,Varname);
}
