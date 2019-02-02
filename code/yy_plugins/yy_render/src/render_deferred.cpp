


// 延迟渲染
// http://wiki.jikexueyuan.com/project/modern-opengl-tutorial/tutorial35.html

// 几何阶段， 利用mrt(multi render target)，将不同的顶点属性一次性输出到不同的纹理中:gbuffer
//geometryPass();

// 光照阶段:遍历gbuffer中每个像素，对不同纹理进行采样以获得像素属性， 对每个像素进行光照计算。
// 通过在屏幕上渲染一个四边形来实现
//LightingPass();
//skyBoxPass();

// 后处理 
//postEffectPass();
//clearCommands();

//#include "scene.h"
//#include "yy_world/inc/components/i_comp_camera.h"
// 
// void Scene::DeferredRendering()
// {
//     int nSize = m_pRenderInfos.size();
//     for(int i=0; i<nSize; i++)
//     {
//         // 渲染一次完整的整个场景, water reflect, 第二个摄像机视口之类的使用
//         const RenderInfo* pRI = m_pRenderInfos[i];
//         DeferredRendering(pRI);
//     }
// 
//     // 渲染一次完整的整个场景
//     DeferredRendering(&m_normalRI);
// }
// 
// void Scene::DeferredRendering(const RenderInfo* pRI)
// {
//     // shadow pass
//     m_pShadowMap->GenerateShadowMap(m_curRendingGameObjs);
// 
//     // gles 2.0 不支持gbuffer?
//     //GeometryPass();
//     // light pass
//     // post effect
// 
// //    RenderTexture(NULL);
// }
