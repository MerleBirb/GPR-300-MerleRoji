// Merle Roji

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace TransformFunctions
{
	// matrix[column][row]

	glm::mat4 scale(glm::vec3 s);

	glm::mat4 rotate(glm::quat r);

	glm::mat4 viewRotate(glm::vec3 r, glm::vec3 u, glm::vec3 f);

	glm::mat4 translate(glm::vec3 p);

	glm::mat4 orthographicProjection(float height, float aspectRatio, float nearPlane, float farPlane);
}