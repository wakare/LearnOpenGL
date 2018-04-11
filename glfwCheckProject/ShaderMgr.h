#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include <GL/glew.h>

enum ShaderType
{
	eVertexShader = GL_VERTEX_SHADER,
	eFragmentShader = GL_FRAGMENT_SHADER,
};

class ShaderMgr
{
private:
	bool bIsOpenCompileErrorLog = true;

	std::map<ShaderType, std::shared_ptr<std::string>> m_szShaderTextVec;
	std::map<ShaderType, const char*> m_szShaderPathVec;
	std::vector<GLuint>	m_LinkedShaderProgram;
public:

	ShaderMgr();
	~ShaderMgr();
	bool Init();
	bool UnInit();
	bool InitShaderPath();
	bool InitShaderSourceFile();

	bool CompileShader();
	bool LinkProgram(GLuint shaderProgram);
};