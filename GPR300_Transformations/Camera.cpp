#include "Camera.h"

Camera::Camera(float aspectRatio)
{
	mPosition = glm::vec3(0, 0, 20);
	mTarget = glm::vec3(0);
	mFOV = 60.0f;
	mOrthographicSize = 7.5f;
	mAspectRatio = aspectRatio;
	mIsOrthographic = false;
	mNearPlane = 0.001f;
	mFarPlane = 1000.0f;
}

glm::mat4 Camera::getViewMatrix()
{
	glm::mat4 viewMatrixTranslation = TransformFunctions::translate(-mPosition);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 forward = glm::normalize(mTarget - mPosition);
	glm::vec3 right = glm::normalize(glm::cross(forward, up));

	up = glm::normalize(glm::cross(right, forward));
	forward = -forward; // flip 

	glm::mat4 viewMatrixRotation = TransformFunctions::viewRotate(right, up, forward);

	return viewMatrixRotation * viewMatrixTranslation;
}

glm::mat4 Camera::getProjectionMatrix()
{
	glm::mat4 projectionMatrix;

	if (mIsOrthographic)
	{
		float width = mOrthographicSize * mAspectRatio;
		float right = width * 0.5f;
		float left = -right;
		float top = mOrthographicSize * 0.5f;
		float bottom = -top;

		projectionMatrix = glm::ortho(left, right, top, bottom, mNearPlane, mFarPlane);
	}
	else
	{
		projectionMatrix = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearPlane, mFarPlane);
	}

	return projectionMatrix;
}