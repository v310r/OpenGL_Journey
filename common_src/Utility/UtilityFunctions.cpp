#include "UtilityFunctions.h"

#include "glad/glad.h"

#include "Core/Core.h"
#include "Texture/CubemapTexture.h"


glm::vec3 UtilityFunctions::GetForwardDirection(const glm::mat4 rotationMatrix)
{
	glm::vec3 forward = -glm::vec3(rotationMatrix[0][2], rotationMatrix[1][2], rotationMatrix[2][2]);
	return forward;
}

glm::mat4 UtilityFunctions::CalculateTransformMatrix(const STransform& transform)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, transform.Translation);
	model = glm::rotate(model, glm::radians(transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, transform.Scale);
	return model;
}

int32_t UtilityFunctions::ConvertCubemapTextureOrientationToOpenGL(CubemapTextureOrientation inOrientation)
{
	switch (inOrientation)
	{
		case CubemapTextureOrientation::Right:	return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		case CubemapTextureOrientation::Left:	return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		case CubemapTextureOrientation::Top:	return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		case CubemapTextureOrientation::Bottom:	return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		case CubemapTextureOrientation::Back:	return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		case CubemapTextureOrientation::Front:	return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

		default: return -1;
	}
}
