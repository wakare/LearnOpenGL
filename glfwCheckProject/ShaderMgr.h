#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include <GL/glew.h>

#include "Shader.h"

class ShaderMgr
{
private:
	bool bIsOpenCompileErrorLog = true;

	std::map<ShaderType, std::shared_ptr<Shader>> m_shaderObjects;
	std::map<ShaderType, const char*> m_szShaderPathVec;
	std::vector<GLuint>	m_LinkedShaderProgram;

public:
	ShaderMgr();
	~ShaderMgr();
	bool Init();
	bool UnInit();
	bool InitShaderPath();
	bool InitShaderObject();

	bool CompileShader();
	bool LinkProgram(GLuint shaderProgram);
};