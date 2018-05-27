#include "TextureMgr.h"
#include "GlobalDefine.h"
#include <iostream>

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

bool TextureMgr::genTextureObjects()
{
	pTexture = (GLuint *)malloc(sizeof(GLuint) * GetTextureCount());
	if (pTexture == nullptr)
	{
		std::cout << "[ERROR] Malloc texture memory failed" << std::endl;
		return false;
	}

	glGenTextures(GetTextureCount(), pTexture);
	return true;
}

GLuint* TextureMgr::GetTextureObjects()
{
	return pTexture;
}