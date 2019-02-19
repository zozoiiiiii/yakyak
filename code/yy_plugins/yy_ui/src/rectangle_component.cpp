#include "rectangle_component.h"
#include "item.h"

RectangleComponent::RectangleComponent(){}

void RectangleComponent::OnEvent(const std::string& event, const YY::VarList& args)
{
	if (event == "addComponent")
	{
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);
		Item* pItem = FindItem();
		if(pItem)
			pItem->SetTransformChanged(true);
	}
}

void RectangleComponent::SetColor(unsigned int color)
{
	m_color = color;
}

unsigned int  RectangleComponent::GetColor()
{
	return m_color;
}

void RectangleComponent::OnAddBatch(IBatchGroup* pBatchGroup)
{
	pBatchGroup->AddGUIBatch(this);
}

void RectangleComponent::Setup()
{
	Item* pItem = FindItem();
	if (!pItem)
		return;

	float x1 = pItem->GetAbsLeft();
	float y1 = pItem->GetAbsTop();
	float x2 = x1 + pItem->GetWidth();
	float y2 = y1 + pItem->GetHeight();

	//x2 = x2 / IRender::Instance()->GetDeviceWidth();
	//y2 = y2 / IRender::Instance()->GetDeviceHeight();
	//x1 = SETX(x1);
	//y1 = SETY(y1);
	//x2 = SETX(x2);
	//y2 = SETY(y2);
	m_vertices[0].pos = YY::Vec3f(x1, y1, 0);
	m_vertices[1].pos = YY::Vec3f(x1, y2, 0);
	m_vertices[2].pos = YY::Vec3f(x2, y1, 0);
	m_vertices[3].pos = YY::Vec3f(x2, y2, 0);

	m_vertices[0].color = GetColor();
	m_vertices[1].color = GetColor();
	m_vertices[2].color = GetColor();
	m_vertices[3].color = GetColor();

	// update dynamic vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);


	int indices[] = { 0,1,2,2,1,3 };
	//int indices[] = { 0,2,1,2,1,3 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void RectangleComponent::OnRender(IRender* pRender, RenderContext* pCxt)
{
	if (pCxt->nRenderType != RT_Normal)
		return;

	Item* pItem = FindItem();
	if (!pItem)
		return;

	if (pItem->IsTransformChanged())
		Setup();
	pItem->SetTransformChanged(false);

	static IShader* pShader = NULL;
	if (NULL == pShader)
	{
		std::string vsh = pRender->GetResPath() + "shader\\painter.vsh";
		std::string fsh = pRender->GetResPath() + "shader\\painter.fsh";
		pShader = pRender->GetResMgr()->LoadShader(vsh, fsh);
	}


	// tell gpu begin
	pShader->Begin();

	YY::Mat4f topLeftMatrix = pRender->GetTopLeftMatrix();
	pShader->SetUniformMat4fv("c_topLeftMatrix", 1, topLeftMatrix.m_data);

	// active vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	int vertexLocation = glGetAttribLocation(pShader->GetProgramHandle(), "iPos");
	if (vertexLocation >= 0)
	{
		glEnableVertexAttribArray(vertexLocation);
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	}

	int diffuse_loc = glGetAttribLocation(pShader->GetProgramHandle(), "iDiffuse");
	if (diffuse_loc >= 0)
	{
		glEnableVertexAttribArray(diffuse_loc);

		//unsigned int color = 0x44332211 -> vec4 color[0]=11;color[1]=22;color[2]=33;color[3]=44; color={11,22,33,44} -> 0x a b g r
		// for convience, let the color = 0x a r g b, do a swap in shader: oDiffuse = iDiffuse.zyxw                    -> 0x a r g b
		glVertexAttribPointer(diffuse_loc, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	// tell gpu end.
	pShader->End();
}