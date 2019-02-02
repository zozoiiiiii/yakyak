#include "batch_group.h"


void BatchGroup::OnCreate(const VariantMap& args)
{
    m_pRenderContext = new RenderContext;
}

void BatchGroup::OnDestroy()
{
    
}

RenderContext* BatchGroup::GetRenderContext()
{
    return m_pRenderContext;
}

void BatchGroup::AddNormalBatch(RenderComponent* pBatch)
{
    m_SolidBatchs.push_back(pBatch);
}

void BatchGroup::AddTopmostBatch(RenderComponent* pBatch)
{
    m_TopmostBatchs.push_back(pBatch);
}


void BatchGroup::AddGUIBatch(RenderComponent* pBatch)
{
	m_GUIBatches.push_back(pBatch);
}

void BatchGroup::RenderAllBatch(IRender* pRender)
{
    // 按顺序渲染所有可视对象
    //DrawSolidBatchs(m_pRenderContext);
    //DrawTopmostBatchs(m_pRenderContext);
	DrawGUIBatches(pRender, m_pRenderContext);
	
}

void BatchGroup::ClearAllBatch()
{
    m_SolidBatchs.clear();
    m_TopmostBatchs.clear();
	m_GUIBatches.clear();
}


void BatchGroup::DrawSolidBatchs(IRender* pRender, RenderContext* pCxt)
{
//     glEnable(GL_DEPTH_TEST);
// 
//     int nSize = m_SolidBatchs.size();
//     for(int j=0; j<nSize; j++)
//     {
//         RenderComponent* pBatch = m_SolidBatchs[j];
// 		pBatch->OnRenderBefore(pCxt);
//         pBatch->OnRender(pCxt);
// 		pBatch->OnRenderAfter(pCxt);
//     }
}

void BatchGroup::DrawTopmostBatchs(IRender* pRender, RenderContext* pCxt)
{
//     glDisable(GL_DEPTH_TEST);
// 
//     int nSize = m_TopmostBatchs.size();
//     for(int j=0; j<nSize; j++)
//     {
// 		RenderComponent* pBatch = m_TopmostBatchs[j];
// 		pBatch->OnRenderBefore(pCxt);
// 		pBatch->OnRender(pCxt);
// 		pBatch->OnRenderAfter(pCxt);
//     }
// 
//     glEnable(GL_DEPTH_TEST);
}

void BatchGroup::DrawGUIBatches(IRender* pRender, RenderContext* pCxt)
{
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int nSize = m_GUIBatches.size();
	for (int j = 0; j < nSize; j++)
	{
		RenderComponent* pBatch = m_GUIBatches[j];
		//pBatch->OnRenderBefore(pCxt);
		pBatch->OnRender(pRender, pCxt);
		//pBatch->OnRenderAfter(pCxt);
	}

	glEnable(GL_DEPTH_TEST);
}

