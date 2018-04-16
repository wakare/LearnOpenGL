#include "Texture.h"
#include "GlobalDefine.h"

Texture::Texture(const char* szTexturePath)
{
	m_pTextureData = SOIL_load_image("container.jpg", &m_nWidth, &m_nHeight, 0, SOIL_LOAD_RGB);
	ASSERT(m_pTextureData != nullptr);
}