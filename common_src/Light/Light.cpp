#include "Light.h"


void Light::SetIntensity(float value)
{
	m_Intensity = value;
}

const float& Light::GetIntensity() const
{
	return m_Intensity;
}

const glm::vec3& Light::GetColor() const
{
	return m_Diffuse;
}

void Light::SetColor(const glm::vec3 color)
{
	m_Diffuse = color;
}

const STransform& Light::GetTransform() const
{
	return m_Transform;
}

void Light::SetTransform(const STransform& transform)
{
	m_Transform = transform;
}

void Light::SetTranslation(const glm::vec3& translation)
{
	m_Transform.Translation = translation;
}

void Light::SetRotation(const glm::vec3& rotation)
{
	m_Transform.Rotation = rotation;

}

void Light::SetScale(const glm::vec3& scale)
{
	m_Transform.Scale = scale;
}

void DirectionalLight::SetAmbient(const glm::vec3 value)
{
	m_Ambient = value;
}

const glm::vec3& DirectionalLight::GetAmbient() const
{
	return m_Ambient;
}

void PointLight::SetConstant(float value)
{
	m_Constant = value;
}

const float& PointLight::GetConstant() const
{
	return m_Constant;
}

void PointLight::SetLinear(float value)
{
	m_Linear = value;
}

const float& PointLight::GetLinear() const
{
	return m_Linear;
}

void PointLight::SetQuadratic(float value)
{
	m_Quadratic = value;
}

const float& PointLight::GetQuadratic() const
{
	return m_Quadratic;
}

void SPointLightCube::SetColor(glm::vec3 value)
{
	MaterialData->SetDiffuse(value);
	PointLightData.SetColor(value);
}

void SpotLight::SetDirection(const glm::vec3& value)
{
	m_Direction = value;
}

const glm::vec3& SpotLight::GetDirection() const
{
	return m_Direction;
}

void SpotLight::SetInnerAngle(float value)
{
	m_InnerConeAngle = value;
}

float SpotLight::GetInnerAngle() const
{
	return m_InnerConeAngle;
}

void SpotLight::SetOuterAngle(float value)
{
	m_OuterConeAngle = value;
}

float SpotLight::GetOuterAngle() const
{
	return m_OuterConeAngle;
}
