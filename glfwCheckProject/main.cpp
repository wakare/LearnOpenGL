#pragma once
#include <iostream>
#include <Windows.h>

// GLEW & GLFW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GlobalDefine.h"
#include "Color.h"
#include "ShaderMgr.h"
#include "GlobalState.h"
#include "Texture.h"

// GLFW Key callBack function
// nAction = {`GLFW_PRESS`, `GLFW_RELEASE`, `GLFW_REPEAT`}
void KeyCallBackFunction(GLFWwindow* pWindow,int nKey,int nScanCode,int nAction,int nMode)
{
	// Esc => Exit Application
	if (nKey == GLFW_KEY_ESCAPE && nAction == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(pWindow, GL_TRUE);
	}

	if (nKey == GLFW_KEY_SPACE && nAction == GLFW_PRESS)
	{
		//if ((GlobalState.Instance()->m_ePolygonMode) == GL_FILL)
		//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	}
}

void UpdateBackupColor(Color_t* pColor)
{
	/*pColor->fRed	= rand() / (float)RAND_MAX;
	pColor->fGreen	= rand() / (float)RAND_MAX;
	pColor->fBlue	= rand() / (float)RAND_MAX;
	pColor->fAlpha	= 1.0f;*/

	pColor->fRed	= (pColor->fRed <= 0.0f)	? 1.0f : pColor->fRed - 0.01f;
	pColor->fGreen	= (pColor->fGreen <= 0.0f)	? 1.0f : pColor->fGreen - 0.02f;
	pColor->fBlue	= (pColor->fBlue <= 0.0f)	? 1.0f : pColor->fBlue - 0.03f;
	pColor->fAlpha	= 1.0f;
}

void RenderBackup(float nRed, float nGreen, float nBlue,float nAlpha, GLbitfield bufferMask)
{
	glClearColor(nRed, nGreen, nBlue, nAlpha);
	glClear(bufferMask);
}

void UpdateUniformVariable(GLuint shaderProgram, const char* uniformName)
{
	GLfloat timeValue = glfwGetTime();
	GLfloat uniformValue = (sin(timeValue) / 2);
	GLint vertexColorLocation = glGetUniformLocation(shaderProgram, uniformName);
	glUseProgram(shaderProgram);
	glUniform1f(vertexColorLocation, uniformValue);
}

bool AddTexture(std::vector<std::shared_ptr<Texture>>& TextureVec, const char* szTexturePath)
{
	std::shared_ptr<Texture> textureObject = std::make_shared<Texture>(szTexturePath);
	ASSERT(textureObject != nullptr);
	TextureVec.push_back(textureObject);

	return true;
}

int main()
{
	int					nWidth						= 0;
	int					nHeight						= 0;
	unsigned long long	nLastUpdateBackupColorTime	= GetTickCount64();

	GLFWwindow*			pWindow						= nullptr;
	Color_t*			pBackupColor				= new Color_t();
	GLuint				VBO;
	GLuint				VAO;
	GLint				success;
	GLchar				infoLog[512];
	GLuint				shaderProgram;
	ShaderMgr			shaderMgr;

	// Triangle Vertex Info (position & vertexColor)
	std::vector<std::shared_ptr<Texture>>	textureVec;

	// Load Resource
	const char* szTexturePath = "Resources/wall.jpg";
	if (!AddTexture(textureVec, szTexturePath))
	{
		std::cout << "Load resource failed, file path = " << szTexturePath << std::endl;
		return -1;
	}

	GLuint* texture = (GLuint *) malloc(sizeof(GLuint) * textureVec.size());
	glGenTextures(textureVec.size(), texture);
	for (int nIndex = 0; nIndex < textureVec.size(); nIndex ++)
	{
		glBindTexture(GL_TEXTURE_2D, texture[nIndex]);
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
				textureVec[nIndex]->m_nWidth, textureVec[nIndex]->m_nHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureVec[nIndex]->m_pTextureData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		// Clear texture memory
		textureVec[nIndex]->DestroyTexture();
	}

	// Triangle Vertex Info (position only)
	const GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	 // ����
		 // for triangle strip
		 1.0f,  0.5f, 0.0f,  0.5f, 0.5f, 0.5f	 // Test vertex for draw triangle strip
	};

	// Base environment config
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		// Mac need this statement
		// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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
	}

	// Init Shader
	{	
		if (!shaderMgr.Init())
			return -1;

		if (!shaderMgr.CompileShader())
			return -1;

		shaderProgram = glCreateProgram();
		if (!shaderMgr.LinkProgram(shaderProgram))
			return -1;

		// Use Program Object
		glUseProgram(shaderProgram);
	}

	// Define VAO which necessary to core profile
	{	
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Create a buffer to store vertex.
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// Bind the triangle vertex data to buffer.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Tell OpenGL how to explain vertex data
		/* glVertexAttribPointer(
			GLuint index,
			GLint size,
			GLenum type,
			GLboolean normalized,
			GLsizei stride,
			const void* pointer);
		*/

		// Set color format
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);

		// Set position format
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);

		// Unbind VAO
		glBindVertexArray(0);
	}

	// Main Render Loop 
	while (!glfwWindowShouldClose(pWindow))
	{
		glfwPollEvents();

		// Update uniform variable to change triangle color.
		UpdateUniformVariable(shaderProgram, "moveVariable");

		if (GetTickCount64() - nLastUpdateBackupColorTime > 100)
		{
			UpdateBackupColor(pBackupColor);
			nLastUpdateBackupColorTime = GetTickCount64();
		}
		RenderBackup(
			pBackupColor->fRed, pBackupColor->fGreen, pBackupColor->fBlue, pBackupColor->fAlpha,
			GL_COLOR_BUFFER_BIT
		);

		// Now we can use VAO to draw triangle.
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		glfwSwapBuffers(pWindow);
	}

	glfwTerminate();
	return 0;
}