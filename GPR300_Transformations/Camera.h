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
	glm::vec3 mTarget; // world position to look at
	float mFOV; // vertical field of view
	float mOrthographicSize; // height of frustrum in viewspace
	float mAspectRatio;
	float mNearPlane;
	float mFarPlane;
	bool mIsOrthographic; // orthographic = true, perspective = false;

public:

	Camera();
	Camera(glm::vec3 newPos, glm::vec3 newTarget, float newFOV, float newOrthosize, float newAspectRatio, bool isOrtho, float newNearPlane, float newFarPlane);

	glm::vec3 getPosition() { return mPosition; }
	glm::vec3 getTarget() { return mTarget; }
	float getFOV() { return mFOV; }
	float getOrthographicSize() { return mOrthographicSize; }
	float getAspectRatio() { return mAspectRatio; }

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
};