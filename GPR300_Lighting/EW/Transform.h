//Author: Eric Winebrenner

#pragma once
#include <glm/glm.hpp>
#include "ewMath.h"

namespace ew {
	struct Transform {
		glm::vec3 position = glm::vec3(0);
		glm::vec3 rotation = glm::vec3(0);
		glm::vec3 scale = glm::vec3(1);

		glm::mat4 getModelMatrix() {
			return ew::translate(position) * ew::rotateX(rotation.x) * ew::rotateY(rotation.y) * ew::rotateZ(rotation.z) * ew::scale(scale);
		}
		void reset() {
			position = glm::vec3(0);
			rotation = glm::vec3(0);
			scale = glm::vec3(1);
		}
	};
}
