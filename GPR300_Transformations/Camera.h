// Merle Roji

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TransformFunctions.h"

struct Camera
{
private:

	glm::vec3 mPosition;
	glm::vec3 mTarget;
	float mFOV; // vertical field of view
	float mOrthographicSize; // height of frustrum in viewspace
	float mAspectRatio;
	float mNearPlane;
	float mFarPlane;
	bool mIsOrthographic; // orthographic = true, perspective = false;

public:

	Camera(float aspectRatio = 1.7777f);

	// getters
	glm::vec3 getPosition() { return mPosition; }
	glm::vec3 getTarget() { return mTarget; }
	float getFOV() { return mFOV; }
	//glm::vec3 getForward();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	// setters
	void setPosition(const glm::vec3 pos) { mPosition = pos; }
	void setTarget(const glm::vec3 tar) { mTarget = tar; }
	void setFOV(const float FOV) { mFOV = FOV; }
	void setNearPlane(const float np) { mNearPlane = np; }
	void setFarPlane(const float fp) { mFarPlane = fp; }
	void setOrthographicSize(const float size) { mOrthographicSize = size; }
	void setAspectRatio(const float ap) { mAspectRatio = ap; }

};