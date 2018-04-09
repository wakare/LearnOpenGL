#pragma once
#include <iostream>
#include <vector>
#include <memory>

#include <GL/glew.h>

class ShaderMgr
{
private:
	std::vector<std::shared_ptr<std::string>> m_szShaderTextVec;
	std::vector<const char*> m_szShaderPathVec;

public:
	ShaderMgr();
	~ShaderMgr();
	bool Init();
	bool UnInit();
	bool InitShaderPath();
	bool InitShaderSourceFile();

	bool BindShader();
};