#pragma once

#include "glm.hpp"

#include "Core/Core.h"

#include "Material/Material.h"
#include "Buffers/VertexArray.h"


class Light
{
public:

	void SetIntensity(float value);
	const float& GetIntensity() const;

	const glm::vec3& GetColor() const;
	void SetColor(const glm::vec3 color);

	const STransform& GetTransform() const;

	void SetTransform(const STransform& transform);

	void SetTranslation(const glm::vec3& translation);
	void SetRotation(const glm::vec3& rotation);
	void SetScale(const glm::vec3& scale);

protected:
	Light() {};

	// this super sucks xD. But whatever, it just educational stuff)
	// might not be even used sometimes
	STransform m_Transform;

	// usually represents the color of light
	glm::vec3 m_Diffuse = glm::vec3(1.0f);

	float m_Intensity = 0.5f;
};

class DirectionalLight : public Light
{
public:
	void SetAmbient(const glm::vec3 value);
	const glm::vec3& GetAmbient() const;

protected:
	glm::vec3 m_Ambient = glm::vec3(0.1f, 0.1f, 0.1f);
};

class SpotLight : public Light
{
public:
	void SetDirection(const glm::vec3& value);
	const glm::vec3& GetDirection() const;

	void SetInnerAngle(float value);
	float GetInnerAngle() const;

	void SetOuterAngle(float value);
	float GetOuterAngle() const;

protected:
	glm::vec3 m_Direction;

	// half conus angle
	float m_InnerConeAngle = 5.0f;

	// half conus angle
	float m_OuterConeAngle = 15.0f;
};

class PointLight : public Light
{
public:
	void SetConstant(float value);
	const float& GetConstant() const;

	void SetLinear(float value);
	const float& GetLinear() const;

	void SetQuadratic(float value);
	const float& GetQuadratic() const;

protected:
	float m_Constant = 1.0f, m_Linear = 0.09f, m_Quadratic = 0.032f;
};

struct SPointLightCube
{
	void SetColor(glm::vec3 value);

	STransform Transform;
	PointLight PointLightData;

	std::shared_ptr<SimpleMaterial> MaterialData;

	std::shared_ptr<VertexArray> VAO;
};
