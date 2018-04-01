#pragma once
#include <iostream>
#include <Windows.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// GLFW Key callBack function
// nAction = {`GLFW_PRESS`, `GLFW_RELEASE`, `GLFW_REPEAT`}
void KeyCallBackFunction(GLFWwindow* pWindow,int nKey,int nScanCode,int nAction,int nMode)
{
	// Esc => Exit Application
	if (nKey == GLFW_KEY_ESCAPE && nAction == GLFW_RELEASE)
	{
		glfwSetWindowShouldClose(pWindow, GL_TRUE);
	}
}

struct Color_t
{
	float fRed;
	float fGreen;
	float fBlue;
	float fAlpha;

	Color_t(): fRed(0),fGreen(0),fBlue(0),fAlpha(0) {}
};

void UpdateBackupColor(Color_t* pColor)
{
	/*pColor->fRed	= rand() / (float)RAND_MAX;
	pColor->fGreen	= rand() / (float)RAND_MAX;
	pColor->fBlue	= rand() / (float)RAND_MAX;
	pColor->fAlpha	= 1.0f;*/

	pColor->fRed	= (pColor->fRed <= 0.0f) ? 1.0f: pColor->fRed - 0.01f;
	pColor->fGreen	= (pColor->fGreen <= 0.0f) ? 1.0f : pColor->fGreen - 0.02f;
	pColor->fBlue	= (pColor->fBlue <= 0.0f) ? 1.0f : pColor->fBlue - 0.03f;
	pColor->fAlpha	= 1.0f;
}

void RenderBackup(float nRed, float nGreen, float nBlue,float nAlpha, GLbitfield bufferMask)
{
	glClearColor(nRed, nGreen, nBlue, nAlpha);
	glClear(bufferMask);
}

int main()
{
	int					nWidth						= 0;
	int					nHeight						= 0;
	unsigned long long	nLastUpdateBackupColorTime	= GetTickCount64();

	GLFWwindow*			pWindow						= nullptr;
	Color_t*			pBackupColor				= new Color_t();

	// Triangle Vertex Info (position only)
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	pWindow = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (pWindow == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(pWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	
	glfwGetFramebufferSize(pWindow, &nWidth, &nHeight);

	glViewport(0, 0, nWidth, nHeight);

	glfwSetKeyCallback(pWindow, KeyCallBackFunction);

	// Create a buffer to store vertex.
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Bind the triangle vertex data to buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* shaderText = 
		"#version 330 core\nlayout(location = 0) in vec3 position;\nvoid main()\n{\ngl_Position = vec4(position.x, position.y, position.z, 1.0);\n}";

	const GLchar* vertexShaderSource	= shaderText;
	const GLchar* vertexShaderArray[1]	= { vertexShaderSource };

	glShaderSource(vertexShader, 1, vertexShaderArray, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	while (!glfwWindowShouldClose(pWindow))
	{
		glfwPollEvents();

		if (GetTickCount64() - nLastUpdateBackupColorTime > 100)
		{
			UpdateBackupColor(pBackupColor);
			nLastUpdateBackupColorTime = GetTickCount64();
		}
		RenderBackup(
			pBackupColor->fRed,pBackupColor->fGreen,pBackupColor->fBlue,pBackupColor->fAlpha, 
			GL_COLOR_BUFFER_BIT
		);

		glfwSwapBuffers(pWindow);
	}

	glfwTerminate();
	return 0;
}