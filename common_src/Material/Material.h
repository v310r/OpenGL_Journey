#pragma once

#include <memory>
#include <string_view>
#include "glm.hpp"


std::string_view GetAmbientName();
std::string_view GetDiffuseName();
std::string_view GetSpecularName();
std::string_view GetEmissionName();
std::string_view GetShininessName();

class Shader;
class Texture;

class Material
{
public:

	Material() = default;

	Material(const std::shared_ptr<Shader>& shader);

	void Bind();

	void Unbind();

	const std::shared_ptr<Texture>& GetDiffuse() const;
	const std::shared_ptr<Texture>& GetSpecular();
	float& GetShininess();

	void SetDiffuse(const std::shared_ptr<Texture>& texture);
	void SetSpecular(const std::shared_ptr<Texture>& texture);
	void SetEmission(const std::shared_ptr<Texture>& texture);

	void SetShininess(float value);

	void SetShader(const std::shared_ptr<Shader>& shader);

protected:
	std::shared_ptr<Texture> m_DiffuseTexture;
	std::shared_ptr<Texture> m_SpecularTexture;
	std::shared_ptr<Texture> m_EmissionTexture;
	float m_Shininess = 0.1f;

	std::shared_ptr<Shader> m_ShaderRef;
};

class SimpleMaterial
{
public:

	SimpleMaterial() = default;

	SimpleMaterial(const std::shared_ptr<Shader>& shader);

	void Bind();

	void Unbind();

	glm::vec3& GetAmbient();
	glm::vec3& GetDiffuse();
	glm::vec3& GetSpecular();
	float& GetShininess();

	void SetAmbient(glm::vec3 value);
	void SetDiffuse(glm::vec3 value);
	void SetSpecular(glm::vec3 value);
	void SetShininess(float value);

	void SetShader(const std::shared_ptr<Shader>& shader);

protected:
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	float m_Shininess = 0.1f;

	std::shared_ptr<Shader> m_ShaderRef;
};