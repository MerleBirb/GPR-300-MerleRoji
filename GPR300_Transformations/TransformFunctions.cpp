#include "TransformFunctions.h"

glm::mat4 TransformFunctions::scale(glm::vec3 s)
{
	glm::mat4 scaleMat = glm::mat4(1);

	scaleMat[0][0] = s.x;
	scaleMat[1][1] = s.y;
	scaleMat[2][2] = s.z;
	scaleMat[3][3] = 1.0f;

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
		r.x, u.x, f.x, 0.0f, // column 1
		r.y, u.y, f.y, 0.0f, // column 2
		r.z, u.z, f.z, 0.0f, // column 3
		0.0f,0.0f,0.0f,1.0f  // column 4
		); 

	return rotateMat;
}

glm::mat4 TransformFunctions::translate(glm::vec3 p)
{
	glm::mat4 translateMat = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f, // column 1
		0.0f, 1.0f, 0.0f, 0.0f, // column 2
		0.0f, 0.0f, 1.0f, 0.0f, // column 3
		p.x,  p.y,  p.z,  1.0f  // column 4
	);

	return translateMat;
}