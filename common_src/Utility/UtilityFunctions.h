#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"


struct STransform;

enum class CubemapTextureOrientation :uint8_t;

namespace UtilityFunctions
{
	// we treat Z axis as forward one
	glm::vec3 GetForwardDirection(const glm::mat4 rotationMatrix);

	glm::mat4 CalculateTransformMatrix(const STransform& transform);

	int32_t ConvertCubemapTextureOrientationToOpenGL(CubemapTextureOrientation inOrientation);
}