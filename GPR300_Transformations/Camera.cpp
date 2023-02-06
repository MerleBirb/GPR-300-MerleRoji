#include "Camera.h"

Camera::Camera()
{
	mPosition = glm::vec3(0);
	mTarget = glm::vec3(0);
	mFOV = 60;
	mOrthographicSize = 1;
	mAspectRatio = 1.77;
	mIsOrthographic = false;
	mNearPlane = 0.001;
	mFarPlane = 1000;
}

Camera::Camera(glm::vec3 newPos, glm::vec3 newTarget, float newFOV, float newOrthosize, float newAspectRatio, bool isOrtho, float newNearPlane, float newFarPlane)
{
	mPosition = newPos;
	mTarget = newTarget;
	mFOV = newFOV;
	mOrthographicSize = newOrthosize;
	mAspectRatio = newAspectRatio;
	mIsOrthographic = isOrtho;
	mNearPlane = newNearPlane;
	mFarPlane = newFarPlane;
}

glm::mat4 Camera::getViewMatrix()
{
	glm::mat4 viewMatrixTranslation = TransformFunctions::translate(-mPosition);

	glm::vec3 right = glm::vec3(mTarget.x, 0, 0);
	glm::vec3 up = glm::vec3(0, mTarget.y, 0);
	glm::vec3 forward = glm::vec3(0, 0, -mTarget.z);

	glm::mat4 viewMatrixRotation = TransformFunctions::viewRotate(right, up, forward);

	return viewMatrixTranslation * viewMatrixRotation;
}

glm::mat4 Camera::getProjectionMatrix()
{
	glm::mat4 projectionMatrix;

	if (mIsOrthographic)
	{
		glm::mat4 orthoMatrix = 
			TransformFunctions::orthographicProjection(mOrthographicSize, mAspectRatio, mNearPlane + mPosition.z, mFarPlane + mPosition.z);

		projectionMatrix = orthoMatrix;
	}
	else
	{
		glm::mat4 perspMatrix =
			TransformFunctions::perspectiveProjection(mFOV, mAspectRatio, mNearPlane + mPosition.z, mFarPlane + mPosition.z);

		projectionMatrix = perspMatrix;
	}

	return projectionMatrix;
}