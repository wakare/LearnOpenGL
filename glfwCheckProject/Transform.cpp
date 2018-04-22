#include "Transform.h"

Transform::Transform()
{
	// Init standard matrix4
	m_transformMatrix = glm::mat4
	(
		{1.0f, 0.0f, 0.0f, 0.0f}, 
		{0.0f, 1.0f, 0.0f, 0.0f}, 
		{0.0f, 0.0f, 1.0f, 0.0f}, 
		{0.0f, 0.0f, 0.0f, 1.0f}
	);
}

void Transform::Rotate(float fRotateAngle, glm::vec3 Axis)
{
	glm::rotate(m_transformMatrix, fRotateAngle, Axis);
}

void Transform::Translation(float fX, float fY, float fZ)
{
	glm::translate(m_transformMatrix, glm::vec3(fX, fY, fZ));
}

void Transform::Scale(float fX, float fY, float fZ)
{
	glm::scale(m_transformMatrix, glm::vec3(fX, fY, fZ));
}

glm::mat4 Transform::GetTransform()
{
	return m_transformMatrix;
}