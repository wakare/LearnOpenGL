#include "ShaderMgr.h"
#include "GlobalDefine.h"
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

	m_szShaderPathVec.insert(std::make_pair(eVertexShader, szVertexShaderPath));
	m_szShaderPathVec.insert(std::make_pair(eFragmentShader, szFragmentShaderPath));

	return true;
}

bool ShaderMgr::InitShaderSourceFile()
{
	std::ifstream shaderReadStream;
	for (auto szShaderPath : m_szShaderPathVec)
	{
		shaderReadStream.open(szShaderPath.second, std::ios::in);
		if (shaderReadStream.bad())
			continue;

		std::string szSourceText((std::istreambuf_iterator<char>(shaderReadStream)),
			std::istreambuf_iterator<char>());
		m_szShaderTextVec.insert(std::make_pair(szShaderPath.first, std::make_shared<std::string>(szSourceText)));

		shaderReadStream.close();
	}

	return true;
}

bool ShaderMgr::Init()
{
	if (!InitShaderPath())
		return false;

	if (!InitShaderSourceFile())
		return false;
}

bool ShaderMgr::UnInit()
{
	return true;
}

bool ShaderMgr::CompileShader()
{
	for (auto shaderPair : m_szShaderTextVec)
	{
		auto shader = glCreateShader(shaderPair.first);
		GLint success;

		const GLchar* ShaderSource = shaderPair.second->c_str();
		const GLchar* vertexShaderArray[1] = { ShaderSource };

		glShaderSource(shader, 1, vertexShaderArray, nullptr);
		glCompileShader(shader);
		glGetShaderiv(eVertexShader, GL_COMPILE_STATUS, &success);

		m_LinkedShaderProgram.push_back(shader);

		if (bIsOpenCompileErrorLog)
		{
			GLchar infoLog[512];

			// Print Compile Error
			if (!success)
			{
				glGetShaderInfoLog(eVertexShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}

		ASSERT(success);
		if (!success)
			return false;
	}
	
	return true;
}

bool ShaderMgr::LinkProgram(GLuint shaderProgram)
{
	GLint success = false;
	GLchar infoLog[512];

	if (m_LinkedShaderProgram.size() <= 0)
		return false;

	// Link shader program
	for (auto shader : m_LinkedShaderProgram)
	{
		glAttachShader(shaderProgram, shader);
	}
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!bIsOpenCompileErrorLog)
		return success;

	// Print Link Error
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
	}

	ASSERT(success);
	return success;
}



