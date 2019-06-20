#include "font_manager.h"
#include "yy_render/inc/i_render.h"
NS_YY_BEGIN
FontManager* FontManager::Instance()
{
	static FontManager s_font_manager;
	return &s_font_manager;
}

void FontManager::Init(IObjectMgr* pObjMgr)
{
	m_pObjMgr = pObjMgr;

	m_library = new FT_Library();
	FT_Init_FreeType(m_library);

	m_defaultFont = GetFont("fonts/arial.ttf", 20);
}

Font* FontManager::GetFont(std::string fontFile, unsigned int fontSize)
{
	char tmp[50];
	std::string uStr = fontFile;
	uStr.append(itoa(fontSize, tmp, 10));
	auto result = m_fontMap.find(uStr);
	if (result != m_fontMap.end())
	{
		return result->second;
	}
	else
	{
		FT_Face* face = new FT_Face();

		std::string strFontFile = IRender::Instance(m_pObjMgr)->GetResPath() + fontFile;
		int bRet = FT_New_Face(*m_library, strFontFile.c_str(), 0, face);

		// For Some Twisted Reason, FreeType Measures Font Size
	// In Terms Of 1/64ths Of Pixels.  Thus, To Make A Font
	// h Pixels High, We Need To Request A Size Of h*64.
	// (h << 6 Is Just A Prettier Way Of Writing h*64)
		//FT_Set_Char_Size(face, h << 6, h << 6, 96, 96);
		FT_Set_Char_Size(*face, fontSize<<6,0, 96, 96);


		Font * font = new Font(fontSize, face, m_pObjMgr);
		m_fontMap.insert(std::make_pair(uStr, font));
		return font;
	}

}

Font* FontManager::GetDefaultFont() const
{
	return m_defaultFont;
}

NS_YY_END