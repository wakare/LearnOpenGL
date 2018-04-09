#include "ShaderMgr.h"
#include <fstream>

ShaderMgr::ShaderMgr()
{
	m_szShaderPathVec.clear();
	m_szShaderTextVec.clear();

	if (!Init())
		return;
}

ShaderMgr::~ShaderMgr()
{
	UnInit();
}

bool ShaderMgr::InitShaderPath()
{
	auto szVertexShaderPath = "Resources/VertexShader.glsl";
	auto szFragmentShaderPath = "Resources/FragmentShader.glsl";

	m_szShaderPathVec.push_back(szVertexShaderPath);
	m_szShaderPathVec.push_back(szFragmentShaderPath);
}

bool ShaderMgr::InitShaderSourceFile()
{
	std::ifstream shaderReadStream;
	for (auto szShaderPath : m_szShaderPathVec)
	{
		shaderReadStream.open(szShaderPath, std::ios::in);
		if (shaderReadStream.bad())
			continue;

		std::istreambuf_iterator<char> beg(shaderReadStream), end;
		std::string sShaderText(beg, in);

		shaderReadStream.close();
	}
}

bool ShaderMgr::Init()
{


	if (!InitShaderSourceFile())
		return false;
}



