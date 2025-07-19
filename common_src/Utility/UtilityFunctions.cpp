#include "UtilityFunctions.h"

#include "glad/glad.h"

#include "Core/Core.h"
#include "Texture/CubemapTexture.h"

#include "Renderer/RenderCommand.h"
#include "Log.h"


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

int32_t UtilityFunctions::ConvertCubemapTextureOrientationToOpenGL(ECubemapTextureOrientation inOrientation)
{
	switch (inOrientation)
	{
		case ECubemapTextureOrientation::Right:		return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		case ECubemapTextureOrientation::Left:		return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		case ECubemapTextureOrientation::Top:		return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		case ECubemapTextureOrientation::Bottom:	return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		case ECubemapTextureOrientation::Back:		return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		case ECubemapTextureOrientation::Front:		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

		default: return -1;
	}
}

uint32_t UtilityFunctions::ConvertDrawPrimitiveModeToOpenGL(EDrawPrimitiveMode inMode)
{
	switch (inMode)
	{
		case EDrawPrimitiveMode::Points:		return GL_POINTS;
		case EDrawPrimitiveMode::Lines:			return GL_LINES;
		case EDrawPrimitiveMode::Triangles:		return GL_TRIANGLES;
		case EDrawPrimitiveMode::TriangleStrip:	return GL_TRIANGLE_STRIP;
		case EDrawPrimitiveMode::TriangleFan:	return GL_TRIANGLE_FAN;

		LOG_ERROR("Can't convert inMode to meaningful EDrawPrimitiveMode; inMode = {0}", static_cast<int>(inMode));
		default: return (uint32_t)-1;
	}
}
