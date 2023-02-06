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

glm::mat4 TransformFunctions::translate(glm::vec3 p)
{
	glm::mat4 translateMat = glm::mat4(1);

	translateMat[3][0] = p.x;
	translateMat[3][1] = p.y;
	translateMat[3][2] = p.z;

	return translateMat;
}