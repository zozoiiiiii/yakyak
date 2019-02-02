/************************************************************************/
/* 
@author:    junliang
@brief:     ÏÔÊ¾camera previewµÄobj
@time:      11/28/2016
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "render/inc/model/i_mesh.h"
#include "components/mesh_render/comp_mesh_render.h"
#include "comp_model_obj.h"
#include "world/inc/components/i_show_camera_preview.h"

class ShowCameraPreview : public IShowCameraPreview
{
    YY_BEGIN(ShowCameraPreview, "Entity");
    YY_END
public:
    virtual ~ShowCameraPreview(){};
    virtual void OnCreate();
    virtual void OnDestroy();

    virtual void Load(const std::string& model_name, const std::string& model_path);
    virtual void OnRender(RenderContext* pCxt);
private:
    YY::IMesh* m_mesh;
};