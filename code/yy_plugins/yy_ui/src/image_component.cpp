#include "image_component.h"
#include "yy_render/inc/res/i_texture.h"
#include "yy_ui/inc/i_gui.h"

//NS_YY_BEGIN
ImageComponent::ImageComponent()
	: m_pTexture(nullptr) {}
void ImageComponent::OnEvent(const std::string& event, const YY::VarList& args)
{
	if (event == "addComponent")
	{
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);
	}

}


void ImageComponent::OnAddBatch(IBatchGroup* pBatchGroup)
{
	pBatchGroup->AddGUIBatch(this);
}


void ImageComponent::Setup()
{
	Item* pItem = FindItem();
	if (!pItem)
		return;

	float x1 = pItem->GetAbsLeft();
	float y1 = pItem->GetAbsTop();
	float x2 = x1 + pItem->GetWidth();
	float y2 = y1 + pItem->GetHeight();

	m_vertices[0].pos = YY::Vec3f(x1, y1, 0);
	m_vertices[1].pos = YY::Vec3f(x1, y2, 0);
	m_vertices[2].pos = YY::Vec3f(x2, y1, 0);
	m_vertices[3].pos = YY::Vec3f(x2, y2, 0);

	m_vertices[0].tex_coords = YY::Vec2f(0, 0);
	m_vertices[1].tex_coords = YY::Vec2f(0, 1);
	m_vertices[2].tex_coords = YY::Vec2f(1, 0);
	m_vertices[3].tex_coords = YY::Vec2f(1, 1);


	// update dynamic vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);


	int indices[] = { 0,1,2,2,1,3 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void ImageComponent::OnRender(IRender* pRender, RenderContext* pCxt)
{
	Item* pItem = FindItem();
	if (!pItem)
		return;

	if (nullptr == m_pTexture)
	{
		return;
	}

	if (pCxt->nRenderType != RT_Normal)
		return;

	if (pItem->GetWidth() == 0 || pItem->GetHeight() == 0)
	{
		pItem->SetWidth(m_pTexture->GetWidth());
		pItem->SetHeight(m_pTexture->GetHeight());
	}

	if (pItem->IsTransformChanged())
		Setup();
	pItem->SetTransformChanged(false);


	static IShader* pShader = NULL;
	if (NULL == pShader)
	{
		std::string vsh = pRender->GetResPath() + "shader\\painter_image.vsh";
		std::string fsh = pRender->GetResPath() + "shader\\painter_image.fsh";
		pShader = pRender->GetResMgr()->LoadShader(vsh, fsh);
	}


	// tell gpu begin
	pShader->Begin();



	// choose one texture unit in gpu
	glActiveTexture(GL_TEXTURE0);

	// bind the texture to the texture unit
	glBindTexture(GL_TEXTURE_2D, m_pTexture->GetID());
	pShader->SetUniform1i("texture", 0);


	YY::Mat4f topLeftMatrix = pRender->GetTopLeftMatrix();
	pShader->SetUniformMat4fv("c_topLeftMatrix", 1, topLeftMatrix.m_data);

	// active vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// position
	int vertexLocation = glGetAttribLocation(pShader->GetProgramHandle(), "iPos");
	if (vertexLocation >= 0)
	{
		glEnableVertexAttribArray(vertexLocation);
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	}

	// texture coordinate
	int texcoordLocation = glGetAttribLocation(pShader->GetProgramHandle(), "i_texcoord");
	if (texcoordLocation >= 0)
	{
		glEnableVertexAttribArray(texcoordLocation);
		glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	// tell gpu end.
	pShader->End();
}


void ImageComponent::SetImageSource(const std::string& url)
{
	IRender* pRender = IGUI::Instance()->GetRender();
	if (!pRender)
		return;

	IResMgr* pResMgr = pRender->GetResMgr();
	m_pTexture = pResMgr->LoadTexture(url);
}

//NS_YY_END