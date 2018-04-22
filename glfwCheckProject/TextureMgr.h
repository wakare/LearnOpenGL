#pragma once
#include "Texture.h"
#include <vector>
#include <memory>

class TextureMgr 
{
private:
	std::vector<std::shared_ptr<Texture>> m_textureVec;

public:
	TextureMgr();
	bool AddTexture(const char* szTexturePath);
	std::vector<std::shared_ptr<Texture>> GetTextureVec();
	size_t GetTextureCount();
};