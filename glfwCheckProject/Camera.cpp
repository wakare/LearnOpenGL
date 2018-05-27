#include "Camera.h"

Camera* Camera::_pCamera = nullptr;

Camera::Camera(glm::vec3 cameraPosition)
{
	SetCameraPosition(cameraPosition);
}

Camera::Camera()
{
	glm::vec3 cameraPosition = {0, 0, 0};
	SetCameraPosition(cameraPosition);
}

glm::mat4 Camera::GetViewTransform()
{
	return m_viewTransform;
}

void Camera::SetViewTransform(glm::vec3 look, glm::vec3 up, glm::vec3 right)
{
	look = glm::normalize(look);
	up = glm::normalize(up);
	right = glm::normalize(right);
}

void Camera::LookAtTarget(glm::vec3 targetPosition, glm::vec3 up)
{
	glm::vec3 lookVec = targetPosition - m_cameraPosition;
	glm::vec3 look = glm::normalize(lookVec);
	glm::vec3 right = glm::cross(look, up);

	SetViewTransform(look, up, right);
}

Camera* Camera::GetCamera()
{

	static std::once_flag onceFlag;
	std::call_once(onceFlag, [&] {_pCamera = new Camera(); });
	return _pCamera;
}

void Camera::SetCameraPosition(glm::vec3 cameraPosition)
{
	m_cameraPosition = cameraPosition;
}

void Camera::RotateAlongAxis(float angles, glm::vec3 axisPosition, glm::vec3 axisVec)
{

}