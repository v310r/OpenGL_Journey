#include "UtilityFunctions.h"

#include "Core/Core.h"


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
