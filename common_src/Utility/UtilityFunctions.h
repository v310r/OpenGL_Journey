#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"


struct STransform;

enum class ECubemapTextureOrientation :uint8_t;
enum class EDrawPrimitiveMode;

namespace UtilityFunctions
{
	// we treat Z axis as forward one
	glm::vec3 GetForwardDirection(const glm::mat4 rotationMatrix);

	glm::mat4 CalculateTransformMatrix(const STransform& transform);

	int32_t ConvertCubemapTextureOrientationToOpenGL(ECubemapTextureOrientation inOrientation);

	uint32_t ConvertDrawPrimitiveModeToOpenGL(EDrawPrimitiveMode inMode);
}