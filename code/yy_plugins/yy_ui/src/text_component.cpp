#include "text_component.h"
#include "item_transform_component.h"
#include "yy_core/core/inc/yy_string.h"
#include <algorithm>
#include "yy_core/math/inc/yy_vec2f.h"

//NS_YY_BEGIN

TextComponent::TextComponent():m_pTransform(nullptr), m_atlas(nullptr), m_pFont(nullptr){}
void TextComponent::OnEvent(const std::string& event, const YY::VarList& args)
{
	if (event == "addComponent")
	{
		m_pTransform = (ItemTransformComponent*)GetOwner()->FindComponent("ItemTransformComponent");
		m_nMaxY = 0;
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);
		m_pFont = FontManager::Instance()->GetDefaultFont();

		if (m_pTransform)
			m_pTransform->SetTransformChanged(true);
	}
}



void TextComponent::OnAddBatch(IBatchGroup* pBatchGroup)
{
	pBatchGroup->AddGUIBatch(this);
}

void TextComponent::OnRender(IRender* pRender, RenderContext* pCxt)
{
	if (pCxt->nRenderType != RT_Normal)
		return;

	if (m_pTransform->IsTransformChanged())
		genMesh();
	m_pTransform->SetTransformChanged(false);

	static IShader* pShader = NULL;
	if (NULL == pShader)
	{
		std::string vsh = pRender->GetResPath() + "shader\\painter_font.vsh";
		std::string fsh = pRender->GetResPath() + "shader\\painter_font.fsh";
		pShader = pRender->GetResMgr()->LoadShader(vsh, fsh);
	}


	// tell gpu begin
	pShader->Begin();



	// choose one texture unit in gpu
	glActiveTexture(GL_TEXTURE0);

	// bind the texture to the texture unit
	glBindTexture(GL_TEXTURE_2D, m_atlas->texture()->GetID());
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
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);


	// tell gpu end.
	pShader->End();

}

void TextComponent::genMesh()
{
	if (!m_pTransform)
		return;

	initAtlas();
	//m_mesh->clear();
	m_vertices.clear();
	m_indices.clear();

	unsigned short indices[] = 
	{
		 0,1,2,  0,2,3,
	};


	// without m_nMaxY, then penY will consider as bottom in freetype.
	int penX = m_pTransform->GetAbsLeft();
	int penY = m_pTransform->GetAbsTop() + m_nMaxY;
	int preMaxWidth = 0;
	auto wideCharStr = YY::Utf8ToWStr(m_string);

	// visit every charactor
	for (auto c : wideCharStr)
	{
		auto gNode = m_atlas->getNode(c);

		//        if (!gNode) continue;
		float w = gNode->m_data.width;
		float h = gNode->m_data.rows;


		// this is used when origin in bottom-left
		//int diff = -int(gNode->m_data.rows) + int(gNode->m_data.top);

		// this is used when origin in top-left
		int diff =  int(gNode->m_data.top) ;

		GLfloat xpos = penX + gNode->m_data.left;
		GLfloat ypos = penY - diff;
		YY::Mat4f mat;
		mat.Ident();
		mat.SetTranslate(xpos, ypos, 0);


		Vertex v0;
		v0.pos = mat * YY::Vec3f(0, 0, -1.0f);
		v0.tex_coords = gNode->UV(0.0f, 0.0f);

		Vertex v1;
		v1.pos = mat * YY::Vec3f(w, 0, -1.0f);
		v1.tex_coords = gNode->UV(1.0f, 0.0f);

		Vertex v2;
		v2.pos = mat * YY::Vec3f(w, h, -1.0f);
		v2.tex_coords = gNode->UV(1.0f, 1.0f);

		Vertex v3;
		v3.pos = mat * YY::Vec3f(0, h, -1.0f);
		v3.tex_coords = gNode->UV(0.0f, 1.0f);



		int offset = m_vertices.size();
		//m_mesh->merge(charMesh, mat);
		m_vertices.push_back(v0);
		m_vertices.push_back(v1);
		m_vertices.push_back(v2);
		m_vertices.push_back(v3);

		for (int k = 0; k < 6; k++)
			m_indices.push_back(offset + indices[k]);

		// update to the next font origin.
		penX += gNode->m_data.advance;
		if (c == '\n')
		{
			preMaxWidth = max(penX, preMaxWidth);
			penY -= m_pFont->getFontSize() + 10;
			penX = 0;
		}
	}
	int result_width = max(penX, preMaxWidth);
	YY::Vec2f contentSize = YY::Vec2f(result_width, abs(penY) + m_pFont->getFontSize());

	//setContentSize(vec2(result_width, abs(penY) + m_pFont->getFontSize()));
	//m_mesh->finish();

	int nVertexCnt = m_vertices.size();
	int nIndexCnt = m_indices.size();

	// update dynamic vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, nVertexCnt * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndexCnt * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
}



void TextComponent::initAtlas()
{
	//m_atlas = m_pFont->getGlyphAtlas();
	//return;

	if (!m_atlas)
		m_atlas = new GlyphAtlas();


	m_atlas->cleanUp();
	m_nMaxY = 0;

	auto wideCharStr = YY::Utf8ToWStr(m_string);
	for (auto c : wideCharStr)
	{
		GlyphData data = m_pFont->getGlyphDataFromChar(c);
		m_atlas->addGlyphData(data);

		if(m_nMaxY < data.rows)
			m_nMaxY = data.rows;
	}
	m_atlas->generate();
	m_atlas->generateGLTexture(GetMgr());
}
//NS_YY_END