#include "Shader.h"
#include "GlobalDefine.h"
#include <fstream>

void Shader::UnInit()
{
	m_eShaderType = eInvaildShader;
	m_sShaderText.clear();
}

bool Shader::Init(ShaderType eShaderType, const char* szShaderPath)
{
	m_eShaderType = eShaderType;

	std::ifstream shaderReadStream;
	shaderReadStream.open(szShaderPath, std::ios::in);
	if (shaderReadStream.bad())
	{
		std::cout << "[ERROR] Open shader file failed, szShaderPath = " << szShaderPath << std::endl;
		return false;
	}

	std::string szSourceText((std::istreambuf_iterator<char>(shaderReadStream)),
		std::istreambuf_iterator<char>());
	m_sShaderText = szSourceText;

	shaderReadStream.close();
	return true;
}

Shader::Shader(ShaderType eShaderType, const char* szShaderPath)
{
	ASSERT(Init(eShaderType, szShaderPath));
}