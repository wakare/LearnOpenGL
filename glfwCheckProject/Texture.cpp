#include "Texture.h"
#include "GlobalDefine.h"

Texture::Texture(const char* szTexturePath)
{
	m_pTextureData = SOIL_load_image(szTexturePath, &m_nWidth, &m_nHeight, 0, SOIL_LOAD_RGB);
	ASSERT(m_pTextureData != nullptr);
}

void Texture::DestroyTexture()
{
	if (m_pTextureData != nullptr)
		SOIL_free_image_data(m_pTextureData);
	m_pTextureData = nullptr;
}

Texture::~Texture()
{
	DestroyTexture();
}

