#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"


struct STransform;

namespace UtilityFunctions
{
	// we treat Z axis as forward one
	glm::vec3 GetForwardDirection(const glm::mat4 rotationMatrix);

	glm::mat4 CalculateTransformMatrix(const STransform& transform);
}