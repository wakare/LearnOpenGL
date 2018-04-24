#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
private:
	glm::mat4 m_transformMatrix;

public:
	Transform();
	void Rotate(float fRotateAngle, glm::vec3 Axis);
	void Translate(float fX, float fY, float fZ);
	void Scale(float fX, float fY, float fZ);
	const Transform& operator*= (const Transform& otherTransform);
	Transform operator* (const Transform& otherTransform);

	glm::mat4 GetTransform();
};