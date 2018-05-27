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

void Transform::SetTransformMatrix(glm::mat4 matrix)
{
	m_transformMatrix = matrix;
}

glm::mat4 Transform::GetTransformMatrix()
{
	return m_transformMatrix;
}

// Set Projection matrix (fov --> radians)
bool Transform::SetProjectionTransform(float fov, float aspect, float nearZ, float farZ)
{
	// Check 
	assert(0.0f < nearZ && nearZ < farZ);

	m_transformMatrix = glm::perspective(fov, aspect, nearZ, farZ);
	return true;
}