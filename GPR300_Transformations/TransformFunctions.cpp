#include "TransformFunctions.h"

glm::mat4 TransformFunctions::scale(glm::vec3 s)
{
	glm::mat4 scaleMat = glm::mat4(1);

	scaleMat[0][0] = s.x;
	scaleMat[1][1] = s.y;
	scaleMat[2][2] = s.z;
	scaleMat[3][3] = 1;

	return scaleMat;
}

glm::mat4 TransformFunctions::rotate(glm::quat r)
{
	glm::quat rotateQuat = glm::quat();
	glm::mat4 rotateMat = glm::mat4(0);

	rotateQuat[0] = r.x;
	rotateQuat[1] = r.y;
	rotateQuat[2] = r.z;
	rotateQuat[3] = r.w;

	rotateMat = glm::toMat4(rotateQuat);

	return rotateMat;
}

glm::mat4 TransformFunctions::viewRotate(glm::vec3 r, glm::vec3 u, glm::vec3 f)
{
	glm::mat4 rotateMat = glm::mat4(
		r.x, u.x, f.x, 0, // column 1
		r.y, u.y, f.y, 0, // column 2
		r.z, u.z, f.z, 0, // column 3
		0,   0,   0,   1  // column 4
		); 

	return rotateMat;
}

glm::mat4 TransformFunctions::translate(glm::vec3 p)
{
	glm::mat4 translateMat = glm::mat4(1);

	translateMat[3][0] = p.x;
	translateMat[3][1] = p.y;
	translateMat[3][2] = p.z;

	return translateMat;
}

glm::mat4 TransformFunctions::orthographicProjection(float height, float aspectRatio, float nearPlane, float farPlane)
{
	float width = height * aspectRatio;
	float right = width / 2.0f;
	float left = -right;
	float top = height / 2.0f;
	float bottom = -top;

	glm::mat4 projMatrix = glm::mat4(
		(2.0f / (right - left)), 0, 0, 0,// column 1
		0, (2.0f / (top - bottom)), 0, 0,// column 2
		0, 0, (-2.0f / (farPlane - nearPlane)), 0, 0 // column 3
		(-(right + left) / (right - left)), (-(top + bottom) / (top - bottom)), (-(farPlane + nearPlane) / (farPlane - nearPlane)), 1 // column 4
	);

	return projMatrix;
}

glm::mat4 TransformFunctions::perspectiveProjection(float FOV, float aspectRatio, float nearPlane, float farPlane)
{
	float pi = 3.14159265358979323846f;
	float radianFOV = FOV * (pi / 180);
	float c = tan(radianFOV / 2);

	glm::mat4 projMatrix = glm::mat4(
		(1.0f / (aspectRatio * c)), 0, 0, 0,						      // column 1
		0, (1.0f / c), 0, 0,										      // column 2
		0, 0, (-((farPlane + nearPlane) / (farPlane - nearPlane))), -1,   // column 3
		0, 0, (-((2 * farPlane * nearPlane) / (farPlane - nearPlane))), 1 // column 4
	);

	return projMatrix;
}