#include "Camera.h"

Camera* Camera::g_pCurrentCamera = nullptr;

Camera::Camera(glm::vec3 cameraPosition)
{
	SetCameraPosition(cameraPosition);
}

Camera::Camera()
{
	glm::vec3 cameraPosition = {0, 0, 0};
	SetCameraPosition(cameraPosition);
}

Transform Camera::GetViewTransform()
{
	return m_viewTransform;
}

void Camera::_setViewTransform(glm::vec3 look, glm::vec3 up, glm::vec3 right)
{
	look	= glm::normalize(look);
	up		= glm::normalize(up);
	right	= glm::normalize(right);

	m_lookDirection		= look;
	m_upDirection		= up;
	m_rightDirection	= right;

	auto viewTransformMatrix = glm::lookAt(m_cameraPosition, m_cameraPosition + m_lookDirection, m_upDirection);
	m_viewTransform.SetTransformMatrix(viewTransformMatrix);
}

void Camera::LookAtTarget(glm::vec3 targetPosition, glm::vec3 up)
{
	glm::vec3 lookVec = targetPosition - m_cameraPosition;
	glm::vec3 look = glm::normalize(lookVec);
	glm::vec3 right = glm::cross(look, up);

	_setViewTransform(look, up, right);
}

Camera* Camera::GetCamera()
{
	static std::once_flag onceFlag;
	std::call_once(onceFlag, [&] {g_pCurrentCamera = new Camera(); });
	return g_pCurrentCamera;
}

void Camera::SetCameraPosition(glm::vec3 cameraPosition)
{
	m_cameraPosition = cameraPosition;
}

void Camera::RotateAlongAxis(float angles, glm::vec3 axisPosition, glm::vec3 axisVec)
{

}

void Camera::SelfTransform(glm::vec3 translate)
{
	auto rightTranslate = m_rightDirection * translate.x;
	auto upTranslate	= m_upDirection * translate.y;
	auto lookTranslate	= m_lookDirection * translate.z;
	
	auto cameraTranslate = upTranslate + lookTranslate + rightTranslate;
	m_cameraPosition += cameraTranslate;

	auto viewTransformMatrix = glm::lookAt(m_cameraPosition, m_cameraPosition + m_lookDirection, m_upDirection);
	m_viewTransform.SetTransformMatrix(viewTransformMatrix);
}