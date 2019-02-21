#include "render.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include "model/mesh.h"
#include "model/anim.h"
#include "model/model_res.h"


void Render::OnCreate(const VariantMap& args)
{
    m_pQuad = NULL;
    m_pResMgr = (IResMgr*)GetMgr()->Create("ResMgr");
    m_pResMgr->SetRender(this);
    ModelRes::Instance()->SetRender(this);

	// painter will render after all other batch groups
	m_pUIBatchGroup = (IBatchGroup*)GetMgr()->Create("BatchGroup");

    // this quad used for show the offscreen rt
	SetupQuad();
	GetMgr()->AddExcute(this);
}

void Render::OnDestroy()
{
	GetMgr()->RemoveExcute(this);
}


void Render::OnExcute(float sec)
{
	IEventMgr* pEventMgr = GetMgr()->GetEventMgr();
	pEventMgr->Invoke(GetID(), "AddBatch_VisObjs");	// 3d visible object
	pEventMgr->Invoke(GetID(), "AddBatch_Items");	// 2d ui item
	RenderAll();
}

IResMgr* Render::GetResMgr()
{
    return m_pResMgr;
}

void Render::SetupQuad()
{
    throw_assert(NULL == m_pQuad, "only setup one time.");
    IModelRes* pModelRes = GetModelResMgr();
    m_pQuad = pModelRes->CreateMesh();

    YY::Vertex vertices[4] = {
        YY::Vertex(-1.0,-1.0,0.0,    0.0,0.0),
        YY::Vertex(1.0,-1.0,0.0,     1.0,0.0),
        YY::Vertex(1.0,1.0,0.0,      1.0,1.0),
        YY::Vertex(-1.0,1.0,0.0,     0.0,1.0)
    };


    YY::SubMesh sm;
    for(int i=0; i<4; i++)
    {
        const YY::Vertex& v = vertices[i];
        sm.vertices.push_back(v);
    }

    unsigned short indices[] = {0,1,2,0,2,3};

    for(int i=0; i<6; i++)
    {
        unsigned short index = indices[i];
        sm.indices.push_back(index);
    }

    m_pQuad->AddSubMesh(sm);
}

IModelRes* Render::GetModelResMgr()
{
    return ModelRes::Instance();
}

void Render::CreateDevice()
{
    // create
    glViewport(0,0,m_nDeviceWidth, m_nDeviceHeight);
}

void Render::ResetDevice()
{
    // clear

    // create
    glViewport(0,0,m_nDeviceWidth, m_nDeviceHeight);

    // 重新创建和渲染尺寸一致的纹理
    m_pResMgr->Reset();




	// https://stackoverflow.com/questions/25416820/set-origin-to-top-left-corner-of-screen-in-opengl-es-2
	m_topLeftMatrix.Ident();
	m_topLeftMatrix.m_data[0] = 2.0f / m_nDeviceWidth;
	m_topLeftMatrix.m_data[5] = -2.0f / m_nDeviceHeight;
	m_topLeftMatrix.m_data[12] = -1;
	m_topLeftMatrix.m_data[13] = 1;
}


Mat4f Render::GetTopLeftMatrix()
{
	return m_topLeftMatrix;
}

void Render::AddGroup(IBatchGroup* pBatchGroup)
{
    m_groups.push_back(pBatchGroup);
}

void Render::RenderAll()
{
    // render all groups and clear every frame. some times the group will not add.
    ForwardRendering();

//     switch(m_nRenderType)
//     {
//     case FORWARD_SHADING:
//         ForwardRendering();
//         break;
//     case DEFERRED_SHADING:
//         throw_assert(false, "not support now");
//         //DeferredRendering();
//         break;
//     }

    m_groups.clear();
}

void Render::RenderTexture(IRenderTexture* pRes)
{
    throw_assert(NULL!=m_pQuad, "null check.");
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pRes->GetID());

    static IShader* pMeshShader = NULL;
    if(!pMeshShader)
    {
        std::string res_path = GetResPath();
        std::string vsh = res_path + "shader/quad.vsh";
        std::string fsh = res_path + "shader/quad.fsh";
        pMeshShader = GetResMgr()->LoadShader(vsh.c_str(), fsh.c_str());
    }

    IShader* pShader = pMeshShader;
    pShader->Begin();
    pShader->SetUniform1i("ColorTexture", 0);
    m_pQuad->draw(pShader);
    pShader->End();
}





//给rgba格式
inline void trans_rgba(unsigned char* bits, int width, int height)
{
    int size = width*height;

    for(int i = 0; i < size; i++)
    {
        char tmp = bits[0];
        bits[0] = bits[2];
        bits[2] = tmp;
        bits += 4;
    }
}

inline void trans_rgb(unsigned char* bits, int width, int height)
{
    int size = width*height;

    for(int i = 0; i < size; i++)
    {
        char tmp = bits[0];
        bits[0] = bits[2];
        bits[2] = tmp;
        bits += 3;
    }
}


// gl es 错误检测函数
inline bool TestErr(const char* ident)
{
    int err = glGetError();	
    if(err == GL_NO_ERROR)
        return true;

    switch(err)
    {
    case GL_INVALID_ENUM:
        throw_assert(false, "Err: GL_INVALID_ENUM ");
        break;
    case GL_INVALID_VALUE:
        throw_assert(false, "Err: GL_INVALID_VALUE ");
        break;
    case GL_INVALID_OPERATION:
        throw_assert(false, "Err: GL_INVALID_OPERATION ");
        break;
    case GL_OUT_OF_MEMORY:
        throw_assert(false, "Err: GL_OUT_OF_MEMORY ");
        break;
    default:
        throw_assert(false, "Err: UnKnown ");
        break;
    }
    return false;
}

void Render::ScreenShot(const char* name, int x, int y, int w, int h)
{
    //glBindFramebuffer(GL_FRAMEBUFFER,0);      // slow
    //m_offScreenBuffer->BindForWriting();        // fast?
// 
//     assert(name != NULL);
//     const char* dot = strrchr(name, '.');
//     throw_assert(NULL!=dot, "file check"<<name);
// 
//     FREE_IMAGE_FORMAT format;
//     if (stricmp(dot, ".bmp") == 0)
//     {
//         format = FIF_BMP;
//     }
//     else if (stricmp(dot, ".jpg") == 0)
//     {
//         format = FIF_JPEG;
//     }
//     else if (stricmp(dot, ".png") == 0)
//     {
//         format = FIF_PNG;
//     }
//     else if (stricmp(dot, ".dds") == 0)
//     {
//         format = FIF_DDS;
//     }
//     else
//     {
//         throw_assert(false, "file not support.");
//     }
// 
//     FIBITMAP * bitmap = ::FreeImage_Allocate(w, h, 32);
//     unsigned char* bits = FreeImage_GetBits(bitmap);
// 
// 
//     /* egl 中以下没注释的话，imp_fmt为0x1907, 即GL_RBG,glReadPixels获取不到数据。
//     注释以后就可以，why?
//     //EGL_RED_SIZE,       8,    // 指定rgb中的r大小
//     //EGL_GREEN_SIZE,     8,    // 指定rgb中的g大小
//     //EGL_BLUE_SIZE,      8,    // 指定rgb中的b大小
//     //EGL_ALPHA_SIZE,     8,    // alhpa
//     */
//     int imp_fmt, imp_type;  
//     glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &imp_fmt);  
//     glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE, &imp_type);
// 
// 
//     //BYTE* pixels = new BYTE[4 * w * h];
//     //memset(pixels, 0, sizeof(BYTE)*w*h*4);
// 
//     // http://www.zwqxin.com/archives/opengl/opengl-api-memorandum-2.html
//     // 设置cpu内存上像素格式，防止像素数据不是4的倍数
//     glPixelStorei(GL_PACK_ALIGNMENT, 1);
// 
//     // 从gpu读取像素到cpu内存上, opengl es do not support glReadBuffer, glReadPixels automatically read the currently
//     // active framebuffer.
//     ::glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, bits);
// 
//     bool result = TestErr("SaveScreenToFile");
//     if(format == FIF_JPEG)
//     {
//         FIBITMAP *old_dib = bitmap;
//         bitmap = FreeImage_ConvertTo24Bits(bitmap);
//         FreeImage_Unload(old_dib);
//         bits = FreeImage_GetBits(bitmap);
//         trans_rgb(bits, w, h);
//     }
//     else
//     {	
//         trans_rgba(bits, w, h);
//     }
// 
//     // use frameimage, must do FreeImage_DeInitialise(); or failed here.
//     result = FreeImage_Save(format, bitmap, name);
//     if(result)
//     {
//         result=true;
//     }
//     //throw_assert(FreeImage_Save(format, bitmap, name), "SaveScreenToFile error"<<name);
//     FreeImage_Unload(bitmap);
}