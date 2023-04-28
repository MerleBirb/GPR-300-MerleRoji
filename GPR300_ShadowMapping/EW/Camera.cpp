//Author: Eric Winebrenner

#include "Camera.h"

glm::vec3 Camera::getForward() {
	float yawRad = glm::radians(mYaw);
	float pitchRad = glm::radians(mPitch);

	glm::vec3 forward;
	forward.x = cos(yawRad) * cos(pitchRad);
	forward.y = sin(pitchRad);
	forward.z = sin(yawRad) * cos(pitchRad);
	return forward;
}

glm::mat4 Camera::getProjectionMatrix() {
	if (mOrtho) {
		float width = mOrthoSize * mAspectRatio;
		float right = width * 0.5f;
		float left = -right;
		float top = mOrthoSize * 0.5f;
		float bottom = -top;
		return glm::ortho(left,right,bottom,top, mNearPlane, mFarPlane);
	}
	else {
		return glm::perspective(glm::radians(mFov), mAspectRatio, mNearPlane, mFarPlane);
	}
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(mPosition, mPosition + getForward(), glm::vec3(0,1,0));
}


