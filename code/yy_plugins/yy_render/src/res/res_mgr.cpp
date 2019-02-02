#include "res_mgr.h"

#include "texture/stbi_DDS_aug.h"


IRes* ResMgr::CreateRes(const char* name)
{
    BaseObject* pBaseObject = GetMgr()->Create(name);
	throw_assert(pBaseObject->IsInstanceOf("IRes"), "type check:"<<name);
    IRes* pRes = (IRes*)pBaseObject;
    pRes->SetResMgr(this);
    return pRes;
}
void ResMgr::SetRender(IRender* pRender)
{
    throw_assert(NULL!=pRender, "null check.");
    m_pRender = pRender;
}

ITexture* ResMgr::LoadTexture(const std::string& file)
{
    std::string res_path = m_pRender->GetResPath();
    std::string tex_file = res_path + file;

    int tex_id = -1;
    std::map<std::string , ITexture*>::iterator result = m_textures.find(tex_file);
    if(result!= m_textures.end())
    {
        ITexture* pRes = result->second;
        return pRes;
    }

    return InnerLoadTexture(tex_file);
}

ITexture* ResMgr::InnerLoadTexture(const std::string& file)
{
    int x,y,n;
    void* pData = stbi_load(file.c_str(), &x, &y, &n, 0);

    //int width, height, channels;
    //unsigned char *pData = SOIL_load_image
    //    (
    //        texture_file_name,
    //        &width, &height, &channels,
    //        SOIL_LOAD_AUTO
    //        );
    if(NULL == pData)
        return NULL;

    IStaticTexture* pRes = (IStaticTexture*)GetMgr()->Create("StaticTexture");
    pRes->SetResMgr(this);
	pRes->SetWidth(x);
	pRes->SetHeight(y);

    // STBI_default TEX_FORMAT_default
    pRes->Generate(x,y,n,pData);
    m_textures[file] = pRes;

    stbi_image_free(pData);
    //SOIL_free_image_data(pData);
    return pRes;
}

IShader* ResMgr::LoadShader(const std::string& vsh, const std::string& fsh)
{
    std::string key = vsh + fsh;
    std::map<std::string , IShader*>::iterator result = m_shaders.find(key);
    if(result!= m_shaders.end())
    {
        IShader* pRes = result->second;
        return pRes;
    }

    IShader* pShader = (IShader*)GetMgr()->Create("Shader");
    pShader->SetResMgr(this);
    pShader->Load(vsh.c_str(), fsh.c_str());
    m_shaders[key] = pShader;
    return pShader;
}

void ResMgr::Reset()
{
    std::map<std::string, ITexture*>::iterator itor = m_textures.begin();
    for(; itor != m_textures.end(); itor++)
    {
        ITexture* pRes = itor->second;
        if(pRes->IsInstanceOf("ColorTexture") || 
            pRes->IsInstanceOf("DepthTexture"))
        {
            pRes->Generate();
        }
    }
}