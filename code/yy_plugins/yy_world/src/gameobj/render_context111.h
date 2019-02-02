/************************************************************************/
/* 
@author:    junliang
@brief:     render component
@time:      11/28/2016
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "i_game_obj.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <EGL/egl.h>

//#include "render/shader.h"

// Джх╬иообнд
//class RenderContext
//{
//public:
//    RenderContext(const Transform* camera, const YY::Mat4f& persp, Shader* pShader);
//    void GetViewMatrix(YY::Mat4f* pMatrix);
//    void GetViewMatrixWithOutTransform(YY::Mat4f* pMatrix);
//    void GetProjMatrix(YY::Mat4f* pMatrix);
//
//    Shader* GetShader(){return m_pShader;}
//
//private:
//    Transform m_camera;
//    YY::Mat4f m_persp;
//    Shader* m_pShader;
//};

class RenderContext : public IRenderContext
{
    YY_BEGIN(RenderContext, "IRenderContext");
    YY_END
public:
    virtual void OnCreate();
    virtual void OnDestroy();
    virtual void SetShadowShader(IShader* pShader){m_pShader = pShader;}
    virtual IShader* GetShadowShader(){return m_pShader;}

    // width,height
    virtual int GetWinWidth(){return m_nWinWidth;}
    virtual int GetWinHeight(){return m_nWinHeight;}
    virtual void SetWinSize(int w, int h);

    virtual int GetDeviceWidth(){return m_nDeviceWidth;}
    virtual int GetDeviceHeight(){return m_nDeviceHeight;}
    virtual void SetDeviceSize(int w, int h, bool bWindowSizeChange);

    //res path
    virtual void SetResPath(const std::string& path);
    virtual std::string GetResPath(){return m_res_path;}

    virtual YY::Mat4f GetViewMatrix(){return m_viewM;}
    virtual YY::Mat4f GetProjMatrix(){return m_projM;}
    virtual void SetViewMatrix(YY::Mat4f m);
    virtual void SetProjMatrix(YY::Mat4f m);
    virtual void SetViewMatrixWithOutPos(const ITransform* pTM);
    virtual void SetViewMatrix(const ITransform* pTM);

    virtual void SetRenderType(RenderType type){m_render_type = type;}
    virtual RenderType GetRenderType(){return m_render_type;}
    virtual void SetBool(int index, bool val){m_BoolValues[index]=val;}
    virtual bool GetBool(int index){return m_BoolValues[index];}
    virtual void SetFloat(int index, float val){m_FloatValues[index]=val;}
    virtual float GetFloat(int index){return m_FloatValues[index];}
    virtual void SetVector(int index, float x, float y, float z){m_VectorValues[index]=YY::Vec3f(x,y,z);}
    virtual YY::Vec3f GetVector(int index){return m_VectorValues[index];}
    virtual void SetColor(int index, float x, float y, float z){m_ColorValues[index]=YY::Vec3f(x,y,z);}
    virtual YY::Vec3f GetColor(int index){return m_ColorValues[index];}
private:
    int m_nWinWidth;        // render window size
    int m_nWinHeight;
    int m_nDeviceWidth;     // device size
    int m_nDeviceHeight;

    YY::Mat4f m_viewM;
    YY::Mat4f m_projM;
    IShader* m_pShader;
    std::string m_res_path;
    RenderType m_render_type;

    bool m_BoolValues[MAX_BOOL_NUM];
    float m_FloatValues[MAX_FLOAT_NUM];
    YY::Vec3f m_ColorValues[MAX_COLOR_NUM];
    YY::Vec3f m_VectorValues[MAX_VECTOR_NUM];
};