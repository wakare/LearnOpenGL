#pragma once

#define GLEW_STATIC

#include <iostream>
#include <windows.h>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Color.h"
#include "Transform.h"
#include "GlobalDefine.h"
#include "GlobalState.h"
#include "ShaderMgr.h"
#include "TextureMgr.h"
#include "FPSPrinter.h"
#include "Camera.h"
#include "SceneMgr.h"

// Global variables declaration
static float fFaceAlpha = 0.2f;

// GLFW Key callBack function
// nAction = {`GLFW_PRESS`, `GLFW_RELEASE`, `GLFW_REPEAT`}
void KeyCallBackFunction(GLFWwindow* pWindow,int nKey,int nScanCode,int nAction,int nMode)
{
	static bool keyPressState[1024] = {false};

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

	// Camera movement
	const float cameraMoveSpeed = 0.03f;
	glm::vec3 cameraMovement = {0.0, 0.0, 0.0};

	if (nKey == GLFW_KEY_UP && nAction == GLFW_PRESS)
		keyPressState[GLFW_KEY_UP] = true;

	if (nKey == GLFW_KEY_UP && nAction == GLFW_RELEASE)
		keyPressState[GLFW_KEY_UP] = false;

	if (nKey == GLFW_KEY_DOWN && nAction == GLFW_PRESS)
		keyPressState[GLFW_KEY_DOWN] = true;

	if (nKey == GLFW_KEY_DOWN && nAction == GLFW_RELEASE)
		keyPressState[GLFW_KEY_DOWN] = false;

	if (nKey == GLFW_KEY_LEFT && nAction == GLFW_PRESS)
		keyPressState[GLFW_KEY_LEFT] = true;

	if (nKey == GLFW_KEY_LEFT && nAction == GLFW_RELEASE)
		keyPressState[GLFW_KEY_LEFT] = false;

	if (nKey == GLFW_KEY_RIGHT && nAction == GLFW_PRESS)
		keyPressState[GLFW_KEY_RIGHT] = true;

	if (nKey == GLFW_KEY_RIGHT && nAction == GLFW_RELEASE)
		keyPressState[GLFW_KEY_RIGHT] = false;

	if (nKey == GLFW_KEY_Z && nAction == GLFW_PRESS)
		keyPressState[GLFW_KEY_Z] = true;

	if (nKey == GLFW_KEY_Z && nAction == GLFW_RELEASE)
		keyPressState[GLFW_KEY_Z] = false;

	if (nKey == GLFW_KEY_X && nAction == GLFW_PRESS)
		keyPressState[GLFW_KEY_X] = true;

	if (nKey == GLFW_KEY_X && nAction == GLFW_RELEASE)
		keyPressState[GLFW_KEY_X] = false;

	if(keyPressState[GLFW_KEY_UP] == true)
		cameraMovement.y += cameraMoveSpeed;

	if(keyPressState[GLFW_KEY_DOWN] == true)
		cameraMovement.y -= cameraMoveSpeed;

	if (keyPressState[GLFW_KEY_LEFT] == true)
		cameraMovement.x -= cameraMoveSpeed;

	if (keyPressState[GLFW_KEY_RIGHT] == true)
		cameraMovement.x += cameraMoveSpeed;

	if (keyPressState[GLFW_KEY_Z] == true)
		cameraMovement.z += cameraMoveSpeed;

	if (keyPressState[GLFW_KEY_X] == true)
		cameraMovement.z -= cameraMoveSpeed;

	if (cameraMovement.x != 0.0 || cameraMovement.y != 0.0f || cameraMovement.z != 0.0f)
		Camera::GetCamera()->SelfTransform(cameraMovement);
}

void UpdateBackupColor(Color_t* pColor)
{
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

bool LoadTextureResources(TextureMgr& textureMgr)
{
	// Load Resource.
	const char* szTexturePath = "Resources/wall.jpg";
	if (!textureMgr.AddTexture(szTexturePath))
	{
		std::cout << "[ERROR] Load resource failed, file path = " << szTexturePath << std::endl;
		return false;
	}

	szTexturePath = "Resources/awesomeface.png";
	if (!textureMgr.AddTexture(szTexturePath))
	{
		std::cout << "[ERROR] Load resource failed, file path = " << szTexturePath << std::endl;
		return false;
	}

	return true;
}

// Base environment config.
bool Init(GLFWwindow*& pWindow, int nWindowWidth, int nWindowHeight, int nFrameBufferWidth, int nFrameBufferHeight)
{
	// Set OpenGL version to 3.3, core profile.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Mac need this statement.
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	pWindow = glfwCreateWindow(nWindowWidth, nWindowHeight, "LearnOpenGL", nullptr, nullptr);
	if (pWindow == nullptr)
	{
		std::cout << "[ERROR] Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(pWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "[ERROR] Failed to initialize GLEW" << std::endl;
		return false;
	}

	glfwGetFramebufferSize(pWindow, &nFrameBufferWidth, &nFrameBufferHeight);
	glViewport(0, 0, nFrameBufferWidth, nFrameBufferHeight);
	glfwSetKeyCallback(pWindow, KeyCallBackFunction);
	glEnable(GL_DEPTH_TEST);

	// Init Camera
	{
		auto camera = Camera::GetCamera();
		camera->SetCameraPosition({ 0, 0, -3 });
		camera->LookAtTarget({ 0, 0, 0 }, { 0, 1, 0 });
	}

	return true;
}

int main()
{
	const int			nWindowWidth				= 800;
	const int			nWindowHeight				= 600;
	int					nFrameBufferWidth			= 0;
	int					nFrameBufferHeight			= 0;
	unsigned long long	nLastUpdateBackupColorTime	= GetTickCount64();

	GLFWwindow*				pWindow						= nullptr;
	Color_t*				pBackupColor				= new Color_t();
	std::shared_ptr<Camera>	pCamera						= nullptr;
	GLuint					VBO;
	GLuint					VAO;
	GLuint					shaderProgram;

	FPSPrinter			_FPSPrinter;
	Transform			transform;
	ShaderMgr			shaderMgr;
	TextureMgr			textureMgr;

	// Triangle Vertex Info:
	// vertexCoordination	vertexColor			textureCoordination
	/*
	const GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	0.0f, 0.0f,  // ×óÏÂ
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // ÓÒÏÂ
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,	0.0f, 1.0f,  // ¶¥²¿
		 // for triangle strip
		 0.5f,  0.5f, 0.0f, 0.5f, 0.5f, 0.5f,	1.0f, 1.0f	 // Test vertex for draw triangle strip
	};
	*/

	// Const variables declaration
	const glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	const float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// Init device environment
	if (!Init(pWindow, nWindowWidth, nWindowHeight, nFrameBufferWidth, nFrameBufferHeight))
		return -1;

	if (!LoadTextureResources(textureMgr))
		return -1;

	// Generate texture object.
	textureMgr.genTextureObjects();

	// Bind texture data to texture object.
	for (int nIndex = 0; nIndex < textureMgr.GetTextureCount(); nIndex++)
	{
		glBindTexture(GL_TEXTURE_2D, textureMgr.GetTextureObjects()[nIndex]);
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

		// Use Program Object.
		// glUseProgram(shaderProgram);
	}

	// Define VAO which necessary in core profile.
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Set color format
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		//glEnableVertexAttribArray(1);
		
		// Set texture format
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// Unbind VAO
		glBindVertexArray(0);
	}

	// Main Render Loop.
	while (!glfwWindowShouldClose(pWindow))
	{
		// Send event.
		glfwPollEvents();

		// Render backupground.
		if (GetTickCount64() - nLastUpdateBackupColorTime > 100)
		{
			UpdateBackupColor(pBackupColor);
			nLastUpdateBackupColorTime = GetTickCount64();
		}

		RenderBackupGround(
			pBackupColor->fRed, pBackupColor->fGreen, pBackupColor->fBlue, pBackupColor->fAlpha,
			GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT
		);

		// Set Texture.
		glUseProgram(shaderProgram);
		for (int nIndex = 0; nIndex < textureMgr.GetTextureCount(); nIndex++)
		{
			// Activate texture unit before bind.
			glActiveTexture(GL_TEXTURE0 + nIndex); 
			glBindTexture(GL_TEXTURE_2D, textureMgr.GetTextureObjects()[nIndex]);
			std::string uniformVarName = "ourTexture";
			uniformVarName.append(std::to_string(nIndex + 1));
			glUniform1i(glGetUniformLocation(shaderProgram, uniformVarName.c_str()), nIndex);
		}

		// Now we can use VAO to draw triangle.
		glBindVertexArray(VAO);
		for (int i = 0; i < 10; i++)
		{
			Transform modelTransform;

			// UpdateTransformMatrix(rotateTransform);
			modelTransform.Translate(cubePositions[i].x, cubePositions[i].y, cubePositions[i].z);
			modelTransform.Rotate(glfwGetTime(), glm::vec3(1.0f, 0.5f, 0.0f));

			Transform viewTransform;
			if (pCamera == nullptr)
				pCamera = Camera::GetCamera();

			// TODO: update Camera
			viewTransform = pCamera->GetViewTransform();

			Transform projTransform;
			projTransform.SetProjectionTransform(glm::radians(45.0f), (float)nWindowWidth / (float)nWindowHeight, 0.1f, 100.0f);

			// Update uniform variable.
			UpdateUniformVariable1f(shaderProgram, "fMoveOffset", -0.5f, 0.5f);
			UpdateUniformVariable1f(shaderProgram, "fFaceAlpha");

			SetUniformVariableMatrix(shaderProgram, "viewMatrix", viewTransform.GetTransformMatrix());
			SetUniformVariableMatrix(shaderProgram, "projectionMatrix", projTransform.GetTransformMatrix());

			SetUniformVariableMatrix(shaderProgram, "modelMatrix", modelTransform.GetTransformMatrix());
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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