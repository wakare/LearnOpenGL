#include "TextureMgr.h"
#include "GlobalDefine.h"

TextureMgr::TextureMgr()
{

}

bool TextureMgr::AddTexture(const char* szTexturePath)
{
	bool bResult = false;
	std::shared_ptr<Texture> textureObject = std::make_shared<Texture>(szTexturePath);

	bResult = (textureObject != nullptr);
	ASSERT(bResult);
	m_textureVec.push_back(textureObject);

	return bResult;
}

std::vector<std::shared_ptr<Texture>> TextureMgr::GetTextureVec()
{
	return m_textureVec;
}

size_t TextureMgr::GetTextureCount()
{
	return m_textureVec.size();
}