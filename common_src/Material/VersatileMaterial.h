#pragma once

#include <memory>
#include <string_view>
#include "glm.hpp"
#include <vector>


class Shader;
class Texture;

class VersatileMaterial
{
public:
	VersatileMaterial() = default;

	VersatileMaterial(const std::shared_ptr<Shader>& shader);

	void Bind();

	void Unbind();

	void AddDiffuse(const std::shared_ptr<Texture>& texture);
	void AddSpecular(const std::shared_ptr<Texture>& texture);
	void AddEmission(const std::shared_ptr<Texture>& texture);

	const std::vector<std::shared_ptr<Texture>>& GetDiffuseTextures() const;
	const std::vector<std::shared_ptr<Texture>>& GetSpecularTextures() const;
	const std::vector<std::shared_ptr<Texture>>& GetEmissionTextures() const;

	std::shared_ptr<Texture> GetDiffuseTextureAtIndex(uint32_t index);
	std::shared_ptr<Texture> GetSpecularTextureAtIndex(uint32_t index);
	std::shared_ptr<Texture> GetEmissionTextureAtIndex(uint32_t index);

	void SetShininess(float value);
	float GetShininess() const;

	void SetShader(const std::shared_ptr<Shader>& shader);
	const std::shared_ptr<Shader>& GetShader() const;

protected:
	std::string TryGetTextureNameBasedOnType(const std::shared_ptr<Texture>& texture, uint32_t index, const std::string& defaultName) const;

	std::vector<std::shared_ptr<Texture>> m_DiffuseTextures;
	std::vector<std::shared_ptr<Texture>> m_SpecularTextures;

	std::vector<std::shared_ptr<Texture>> m_EmissionTextures;

	float m_Shininess = 0.1f;

	std::shared_ptr<Shader> m_ShaderRef;
};
