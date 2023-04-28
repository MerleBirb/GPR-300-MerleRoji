//Author: Eric Winebrenner

#pragma once

#include <glm/glm.hpp>

namespace ew {
	glm::mat4 translate(const glm::vec3& t) {
		return glm::mat4{
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			t.x, t.y, t.z, 1.0 
		};
	}

	glm::mat4 rotateX(float a) {
		return glm::mat4{
			1.0,  0.0, 0.0, 0.0,
			0.0, cos(a), sin(a), 0.0,
			0.0, -sin(a), cos(a), 0.0,
			0.0, 0.0, 0.0, 1.0
		};
	}

	glm::mat4 rotateY(float a) {
		return glm::mat4{
			cos(a),  0.0, sin(a), 0.0,
			0.0,     1.0, 0.0,    0.0,
			-sin(a), 0.0, cos(a), 0.0,
			0.0,     0.0, 0.0,    1.0
		};
	}

	glm::mat4 rotateZ(float a) {
		return glm::mat4{
			cos(a),  sin(a), 0.0, 0.0,
			-sin(a), cos(a), 0.0, 0.0,
			0.0, 0.0,        1.0, 0.0,
			0.0, 0.0,        0.0, 1.0
		};
	}

	glm::mat4 scale(const glm::vec3& s) {
		return glm::mat4{
			s.x, 0.0, 0.0, 0.0,
			0.0, s.y, 0.0, 0.0,
			0.0, 0.0, s.z, 0.0,
			0.0, 0.0, 0.0, 1.0
		};
	}
}
