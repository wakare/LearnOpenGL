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

	m_szShaderPathVec.insert(std::make_pair(vertexShader, szVertexShaderPath));
	m_szShaderPathVec.insert(std::make_pair(fragmentShader, szFragmentShaderPath));
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
}

bool ShaderMgr::Init()
{
	if (!InitShaderSourceFile())
		return false;
}

bool ShaderMgr::UnInit()
{

}

bool ShaderMgr::CompileShader()
{
	for (auto shaderPair : m_szShaderTextVec)
	{
		auto shader = glCreateShader(shaderPair.first);

		const GLchar* ShaderSource = shaderPair.second->c_str();
		const GLchar* vertexShaderArray[1] = { ShaderSource };

		glShaderSource(shader, 1, vertexShaderArray, nullptr);
		glCompileShader(shader);

		if (!bIsOpenCompileErrorLog)
			continue;

		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		// Print Compile Error
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
			return -1;
		}
	}
	
	return true;
}

bool ShaderMgr::LinkProgram()
{
	// Link shader program
	auto shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	if (!bIsOpenCompileErrorLog)
		return true;

	GLint success;
	GLchar infoLog[512];

	// Print Link Error
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		return -1;
	}
}



