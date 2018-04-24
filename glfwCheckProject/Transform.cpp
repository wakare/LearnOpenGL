#include "Transform.h"

Transform::Transform()
{
	// Init standard matrix4
	m_transformMatrix = glm::mat4
	(
		1.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

void Transform::Rotate(float fRotateAngle, glm::vec3 Axis)
{
	m_transformMatrix = glm::rotate(m_transformMatrix, fRotateAngle, Axis);
}

void Transform::Translate(float fX, float fY, float fZ)
{
	m_transformMatrix = glm::translate(m_transformMatrix, glm::vec3(fX, fY, fZ));
}

void Transform::Scale(float fX, float fY, float fZ)
{
	m_transformMatrix = glm::scale(m_transformMatrix, glm::vec3(fX, fY, fZ));
}

Transform Transform::operator* (const Transform& otherTransform)
{
	Transform resultTransform;
	resultTransform.m_transformMatrix = this->m_transformMatrix;
	resultTransform.m_transformMatrix *= otherTransform.m_transformMatrix;

	return resultTransform;
}

const Transform& Transform::operator*= (const Transform& otherTransform)
{
	m_transformMatrix *= otherTransform.m_transformMatrix;
	return (*this);
}

glm::mat4 Transform::GetTransform()
{
	return m_transformMatrix;
}