// Merle Roji

#include "Transform.h"

Transform::Transform()
{
	mPosition = glm::vec3(0.0f);
	mRotation = glm::quat();
	mScale = glm::vec3(0.0f);
}

Transform::Transform(glm::vec3 newPos, glm::quat newRot, glm::vec3 newScale)
{
	mPosition = newPos;
	mRotation = newRot;
	mScale = newScale;
}

glm::mat4 Transform::getModelMatrix()
{
	glm::mat4 modelMatrix = getPositionMatrix() * getRotationMatrix() * getScaleMatrix();
	return modelMatrix;
}