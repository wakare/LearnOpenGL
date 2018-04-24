#pragma once
#include <iostream>
#include <windows.h>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Color.h"
#include "Transform.h"
#include "GlobalDefine.h"
#include "GlobalState.h"
#include "ShaderMgr.h"
#include "TextureMgr.h"
#include "FPSPrinter.h"

#define WIDTH		800
#define HEIGHT		600

// Global variables declaration
static float fFaceAlpha = 0.2f;

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
		if (GlobalState::Instance()->m_ePolygonMode == GL_FILL)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			GlobalState::Instance()->m_ePolygonMode = GL_LINE;
		}
		else if (GlobalState::Instance()->m_ePolygonMode == GL_LINE)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			GlobalState::Instance()->m_ePolygonMode = GL_FILL;
		}
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

void UpdateTransformMatrix(Transform& transform)
{
	//transform.Scale(0.99f, 0.99f, 1.0f);
	transform.Rotate(glm::radians(2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//transform.Translate(0.02f, 0.0f, 0.0f);
}

void RenderBackupGround(float nRed, float nGreen, float nBlue,float nAlpha, GLbitfield bufferMask)
{
	glClearColor(nRed, nGreen, nBlue, nAlpha);
	glClear(bufferMask);
}

void SetUniformVariableMatrix(GLuint shaderProgram, const char* uniformName, glm::mat4 transformMatrix)
{
	GLint vertexColorLocation = glGetUniformLocation(shaderProgram, uniformName);
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(vertexColorLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
}

void UpdateUniformVariable1f(GLuint shaderProgram, const char* uniformName, float fStartValue = 0.0f, float fEndValue = 1.0f, 
	std::function<float()> function = []()
	{
		GLfloat timeValue = glfwGetTime();
		return sin(timeValue);
	})
{
	float fAvgValue = (fStartValue + fEndValue) / 2;
	float fScaleValue = (fEndValue - fStartValue) / 2;

	GLfloat uniformValue = fAvgValue + fScaleValue * function();
	GLint vertexColorLocation = glGetUniformLocation(shaderProgram, uniformName);
	glUseProgram(shaderProgram);
	glUniform1f(vertexColorLocation, uniformValue);
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
	GLuint				shaderProgram;
	GLuint*				texture;

	FPSPrinter			_FPSPrinter;
	Transform			transform;
	ShaderMgr			shaderMgr;
	TextureMgr			textureMgr;

	// Triangle Vertex Info 
	// vertexCoordination	vertexColor			textureCoordination
	const GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	0.0f, 0.0f,  // ×óÏÂ
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // ÓÒÏÂ
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,	0.0f, 1.0f,  // ¶¥²¿
		 // for triangle strip
		 0.5f,  0.5f, 0.0f, 0.5f, 0.5f, 0.5f,	1.0f, 1.0f	 // Test vertex for draw triangle strip
	};
	// Base environment config.
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		// Mac need this statement.
		// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		pWindow = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
		if (pWindow == nullptr)
		{
			std::cout << "[ERROR] Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(pWindow);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cout << "[ERROR] Failed to initialize GLEW" << std::endl;
			return -1;
		}

		glfwGetFramebufferSize(pWindow, &nWidth, &nHeight);
		glViewport(0, 0, nWidth, nHeight);
		glfwSetKeyCallback(pWindow, KeyCallBackFunction);
	}

	// Init Coordination transform release
	{
		glm::mat4 model;
		model = glm::rotate(model, -55.0f, glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4 view;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	}

	// Load Resource.
	const char* szTexturePath = "Resources/wall.jpg";
	if (!textureMgr.AddTexture(szTexturePath))
	{
		std::cout << "[ERROR] Load resource failed, file path = " << szTexturePath << std::endl;
		return -1;
	}

	szTexturePath = "Resources/awesomeface.png";
	if (!textureMgr.AddTexture(szTexturePath))
	{
		std::cout << "[ERROR] Load resource failed, file path = " << szTexturePath << std::endl;
		return -1;
	}

	texture = (GLuint *) malloc(sizeof(GLuint) * textureMgr.GetTextureCount());
	if (texture == nullptr)
	{
		std::cout << "[ERROR] Malloc texture memory failed" << std::endl;
		return -1;
	}
	
	// Generate texture object.
	glGenTextures(textureMgr.GetTextureCount(), texture);

	// Bind texture data to texture object.
	for (int nIndex = 0; nIndex < textureMgr.GetTextureCount(); nIndex++)
	{
		glBindTexture(GL_TEXTURE_2D, texture[nIndex]);
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
				textureMgr.GetTextureVec()[nIndex]->m_nWidth, textureMgr.GetTextureVec()[nIndex]->m_nHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureMgr.GetTextureVec()[nIndex]->m_pTextureData);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		// Clear texture memory.
		textureMgr.GetTextureVec()[nIndex]->DestroyTexture();
	}

	// Init Shader.
	{	
		if (!shaderMgr.Init())
			return -1;

		if (!shaderMgr.CompileShader())
			return -1;

		shaderProgram = glCreateProgram();
		if (!shaderMgr.LinkProgram(shaderProgram))
			return -1;

		// Use Program Object
		// glUseProgram(shaderProgram);
	}

	// Define VAO which necessary to core profile.
	{	
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Create a buffer to store vertex.
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// Bind the triangle vertex data to buffer.
		glBufferData(GL_ARRAY_BUFFER,  sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Tell OpenGL how to explain vertex data
		/* glVertexAttribPointer(
			GLuint index,
			GLint size,
			GLenum type,
			GLboolean normalized,
			GLsizei stride,
			const void* pointer);
		*/

		// Set position format
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Set color format
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		
		// Set texture format
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		// Unbind VAO
		glBindVertexArray(0);
	}

	Transform rotateTransform;
	Transform translateTransform;

	translateTransform.Translate(1.0f, 0.0f, 0.0f);

	// Main Render Loop.
	while (!glfwWindowShouldClose(pWindow))
	{
		// Send event.
		glfwPollEvents();

		// Update uniform variable.
		UpdateUniformVariable1f(shaderProgram, "fMoveOffset", -0.5f, 0.5f);
		UpdateUniformVariable1f(shaderProgram, "fFaceAlpha");

		UpdateTransformMatrix(rotateTransform);
		transform = rotateTransform * translateTransform;
		SetUniformVariableMatrix(shaderProgram, "transform", transform.GetTransform());

		// Render backupground.
		if (GetTickCount64() - nLastUpdateBackupColorTime > 100)
		{
			UpdateBackupColor(pBackupColor);
			nLastUpdateBackupColorTime = GetTickCount64();
		}

		RenderBackupGround(
			pBackupColor->fRed, pBackupColor->fGreen, pBackupColor->fBlue, pBackupColor->fAlpha,
			GL_COLOR_BUFFER_BIT
		);

		// Now we can use VAO to draw triangle.
		glUseProgram(shaderProgram);
		for (int nIndex = 0; nIndex < textureMgr.GetTextureCount(); nIndex++)
		{
			// Activate texture unit before bind.
			glActiveTexture(GL_TEXTURE0 + nIndex); 
			glBindTexture(GL_TEXTURE_2D, texture[nIndex]);
			std::string uniformVarName = "ourTexture";
			uniformVarName.append(std::to_string(nIndex + 1));
			glUniform1i(glGetUniformLocation(shaderProgram, uniformVarName.c_str()), nIndex);
		}
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		glfwSwapBuffers(pWindow);

		_FPSPrinter.Update();
	}

	// Properly de-allocate all resources once they've outlived their purpose.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}