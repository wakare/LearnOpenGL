#pragma once
#include "Texture.h"
#include <vector>
#include <memory>
#include <gl/glew.h>

class TextureMgr 
{
private:
	std::vector<std::shared_ptr<Texture>> m_textureVec;
	GLuint* pTexture;

public:
	TextureMgr();
	bool genTextureObjects();
	bool AddTexture(const char* szTexturePath);
	std::vector<std::shared_ptr<Texture>> GetTextureVec();
	size_t GetTextureCount();
	GLuint* GetTextureObjects();
};