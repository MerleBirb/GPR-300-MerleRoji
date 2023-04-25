//Author: Eric Winebrenner
#pragma once
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	Camera(float aspectRatio) : mAspectRatio(aspectRatio) {
	}
	//GETTERS
	inline glm::vec3 getPosition()const { return mPosition; }
	inline float getYaw()const { return mYaw; }
	inline float getPitch()const { return mPitch; }
	inline float getFov()const { return mFov; }
	glm::vec3 getForward();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	//SETTERS
	inline void setPosition(const glm::vec3 position) { mPosition = position; }
	inline void setYaw(const float yaw) { mYaw = yaw; };
	inline void setPitch(const float pitch) { mPitch = pitch; }
	inline void setFov(const float fov) { mFov = glm::clamp(fov, 0.0f, 180.0f); }
	inline void setNearPlane(const float nearPlane) { mNearPlane = nearPlane; }
	inline void setFarPlane(const float farPlane) { mFarPlane = farPlane; }
	inline void setOrthoSize(const float orthoSize) { mOrthoSize = orthoSize; }
	inline void setOrtho(const bool ortho) { mOrtho = ortho; }
	inline void setAspectRatio(const float aspectRatio) { mAspectRatio = aspectRatio; }
private:
	glm::vec3 mPosition = glm::vec3(0, 0, 5);
	float mYaw = -90.0f;
	float mPitch = 0.0f;
	float mFov = 60.0f;
	float mNearPlane = 0.001f;
	float mFarPlane = 1000.0f;
	float mOrthoSize = 7.5f;
	bool mOrtho = false;
	float mAspectRatio = 1.7777f;
};