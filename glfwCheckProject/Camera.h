#pragma once
#include <glm/glm.hpp>
#include <mutex>
#include <memory>
#include "Transform.h"

class Camera
{
private:
	static std::shared_ptr<Camera> g_pCurrentCamera;

	Transform m_viewTransform;
	glm::vec3 m_cameraPosition;
	glm::vec3 m_lookDirection;
	glm::vec3 m_upDirection;
	glm::vec3 m_rightDirection;

	Camera();
	Camera(glm::vec3 cameraPosition);

	void _setViewTransform(glm::vec3 look, glm::vec3 up, glm::vec3 right);

public:
	static std::shared_ptr<Camera> GetCamera();

	void SetCameraPosition(glm::vec3 cameraPosition);
	void RotateAlongAxis(float angles, glm::vec3 axisPosition, glm::vec3 axisVec);
	void LookAtTarget(glm::vec3 targetPosition, glm::vec3 up);
	void SelfTransform(glm::vec3 translate);

	Transform GetViewTransform();
};