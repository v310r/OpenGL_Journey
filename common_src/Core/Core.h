#pragma once

#include "glm.hpp"

#include <memory>


struct STransform
{
	glm::vec3 Translation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

enum class ETextureType : uint8_t
{
	None = 0,
	Diffuse,
	Specular,
	Emission,
	Normal,
	Height,
	Shininess,
	Ambient
};