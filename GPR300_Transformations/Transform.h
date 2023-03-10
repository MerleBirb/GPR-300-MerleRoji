// Merle Roji

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TransformFunctions.h"

struct Transform
{
private:
	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;

public:
	Transform();
	Transform(glm::vec3 newPos, glm::quat newRot, glm::vec3 newScale);
	
	// getters
	glm::vec3 getPosition() { return mPosition; }
	glm::quat getRotation() { return mRotation; }
	glm::vec3 getScale() { return mScale; }
	glm::mat4 getPositionMatrix() { return TransformFunctions::translate(mPosition); }
	glm::mat4 getRotationMatrix() { return TransformFunctions::rotate(mRotation); }
	glm::mat4 getScaleMatrix() { return TransformFunctions::scale(mScale); }

	// setters
	void setPosition(const glm::vec3 p) { mPosition = p; }
	void setRotation(const glm::quat r) { mRotation = r; }
	void setScale(const glm::vec3 s) { mScale = s; }

	glm::mat4 getModelMatrix();
};