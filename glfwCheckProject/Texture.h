#pragma once
#include <SOIL.h>

class Texture
{
public:
	int m_nWidth;
	int m_nHeight;

	unsigned char* m_pTextureData;

	Texture(const char* szTexturePath);
};