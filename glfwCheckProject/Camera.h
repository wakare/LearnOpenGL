#pragma once
#include <glm/glm.hpp>
#include <mutex>

class Camera
{
private:
	static std::mutex _SingletonMutex;
	static Camera* _pCamera;
	glm::mat4 m_viewTransform;
	glm::vec3 m_cameraPosition;

	Camera::Camera();
	Camera(glm::vec3 cameraPosition);

public:
	static Camera* GetCamera();

	void SetCameraPosition(glm::vec3 cameraPosition);
	void RotateAlongAxis(float angles, glm::vec3 axisPosition, glm::vec3 axisVec);
	void LookAtTarget(glm::vec3 targetPosition, glm::vec3 up);
	void SetViewTransform(glm::vec3 look, glm::vec3 up, glm::vec3 right);

	glm::mat4 GetViewTransform();
};