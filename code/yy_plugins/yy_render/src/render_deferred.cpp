


// �ӳ���Ⱦ
// http://wiki.jikexueyuan.com/project/modern-opengl-tutorial/tutorial35.html

// ���ν׶Σ� ����mrt(multi render target)������ͬ�Ķ�������һ�����������ͬ��������:gbuffer
//geometryPass();

// ���ս׶�:����gbuffer��ÿ�����أ��Բ�ͬ������в����Ի���������ԣ� ��ÿ�����ؽ��й��ռ��㡣
// ͨ������Ļ����Ⱦһ���ı�����ʵ��
//LightingPass();
//skyBoxPass();

// ���� 
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
//         // ��Ⱦһ����������������, water reflect, �ڶ���������ӿ�֮���ʹ��
//         const RenderInfo* pRI = m_pRenderInfos[i];
//         DeferredRendering(pRI);
//     }
// 
//     // ��Ⱦһ����������������
//     DeferredRendering(&m_normalRI);
// }
// 
// void Scene::DeferredRendering(const RenderInfo* pRI)
// {
//     // shadow pass
//     m_pShadowMap->GenerateShadowMap(m_curRendingGameObjs);
// 
//     // gles 2.0 ��֧��gbuffer?
//     //GeometryPass();
//     // light pass
//     // post effect
// 
// //    RenderTexture(NULL);
// }
