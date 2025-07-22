#include "VersatileMaterial.h"
#include "Shader1/Shader.h"
#include "Texture/Texture.h"


static std::string_view s_DiffuseName = "material.diffuse";
static std::string_view s_SpecularName = "material.specular";
static std::string_view s_EmissionName = "material.emission";
static std::string_view s_ShininessName = "material.shininess";

static std::string ConvertTextureTypeToTextureName(ETextureType textureType)
{
	switch (textureType)
	{
		case ETextureType::Diffuse:		return "diffuse";
		case ETextureType::Specular:	return "specular";
		case ETextureType::Normal:		return "normal";
		case ETextureType::Height:		return "height";
		case ETextureType::Emission:	return "emission";
		case ETextureType::Shininess:	return "shininess";
		case ETextureType::Ambient:		return "ambient";
		case ETextureType::None:		return "";
		
		default:						return "";
	}
}

static std::string_view GetDiffuseName()
{
	return s_DiffuseName;
}

static std::string_view GetSpecularName()
{
	return s_SpecularName;
}

static std::string_view GetNormalName()
{
	return s_SpecularName;
}

static std::string_view GetEmissionName()
{
	return s_EmissionName;
}

static std::string_view GetShininessName()
{
	return s_ShininessName;
}

VersatileMaterial::VersatileMaterial(const std::shared_ptr<Shader>& shader)
{
	SetShader(shader);
}

void VersatileMaterial::Bind()
{
	m_ShaderRef->Bind();

	for (uint32_t i = 0; i < m_DiffuseTextures.size(); ++i)
	{
		if (!m_DiffuseTextures[i].get())
		{
			continue;
		}

		std::string finalDiffuseTextureName;
		{
			const std::string defaultDiffuseName = std::string(GetDiffuseName());
			finalDiffuseTextureName = TryGetTextureNameBasedOnType(m_DiffuseTextures[i], i, defaultDiffuseName);
		}

		m_ShaderRef->SetInt(finalDiffuseTextureName, m_DiffuseTextures[i]->GetTextureUnit());
		m_DiffuseTextures[i]->BindAndActivateUnit();
	}

	for (uint32_t i = 0; i < m_SpecularTextures.size(); ++i)
	{
		if (!m_SpecularTextures[i].get())
		{
			continue;
		}

		std::string finalSpecularTextureName;
		{
			const std::string defaultSpecularName = std::string(GetSpecularName());
			finalSpecularTextureName = TryGetTextureNameBasedOnType(m_SpecularTextures[i], i, defaultSpecularName);
		}

		m_ShaderRef->SetInt(finalSpecularTextureName, m_SpecularTextures[i]->GetTextureUnit());
		m_SpecularTextures[i]->BindAndActivateUnit();
	}

	for (uint32_t i = 0; i < m_EmissionTextures.size(); ++i)
	{
		if (!m_EmissionTextures[i].get())
		{
			continue;
		}

		std::string finalEmissionTextureName;
		{
			const std::string defaultEmissionName = std::string(GetEmissionName()) + std::to_string(i);
			finalEmissionTextureName = TryGetTextureNameBasedOnType(m_EmissionTextures[i], i, defaultEmissionName);
		}

		m_ShaderRef->SetInt(finalEmissionTextureName, m_EmissionTextures[i]->GetTextureUnit());
		m_EmissionTextures[i]->BindAndActivateUnit();
	}

	m_ShaderRef->SetFloat(GetShininessName(), m_Shininess);
}

void VersatileMaterial::Unbind()
{
	m_ShaderRef->Unbind();
}

void VersatileMaterial::AddDiffuse(const std::shared_ptr<Texture>& texture)
{
	m_DiffuseTextures.emplace_back(texture);
}

void VersatileMaterial::AddSpecular(const std::shared_ptr<Texture>& texture)
{
	m_SpecularTextures.emplace_back(texture);
}

void VersatileMaterial::AddEmission(const std::shared_ptr<Texture>& texture)
{
	m_EmissionTextures.emplace_back(texture);
}

const std::vector<std::shared_ptr<Texture>>& VersatileMaterial::GetDiffuseTextures() const
{
	return m_DiffuseTextures;
}

const std::vector<std::shared_ptr<Texture>>& VersatileMaterial::GetSpecularTextures() const
{
	return m_SpecularTextures;
}

const std::vector<std::shared_ptr<Texture>>& VersatileMaterial::GetEmissionTextures() const
{
	return m_EmissionTextures;
}

std::shared_ptr<Texture> VersatileMaterial::GetDiffuseTextureAtIndex(uint32_t index)
{
	assert(index >= 0 && index < m_DiffuseTextures.size());
	return m_DiffuseTextures[index];
}

std::shared_ptr<Texture> VersatileMaterial::GetSpecularTextureAtIndex(uint32_t index)
{
	assert(index >= 0 && index < m_SpecularTextures.size());
	return m_SpecularTextures[index];
}

std::shared_ptr<Texture> VersatileMaterial::GetEmissionTextureAtIndex(uint32_t index)
{
	assert(index >= 0 && index < m_EmissionTextures.size());
	return m_EmissionTextures[index];
}

void VersatileMaterial::SetShininess(float value)
{
	m_Shininess = value;
}

float VersatileMaterial::GetShininess() const
{
	return m_Shininess;
}

void VersatileMaterial::SetShader(const std::shared_ptr<Shader>& shader)
{
	m_ShaderRef = shader;
}

const std::shared_ptr<Shader>& VersatileMaterial::GetShader() const
{
	return m_ShaderRef;

}

std::string VersatileMaterial::TryGetTextureNameBasedOnType(const std::shared_ptr<Texture>& texture, uint32_t index, const std::string& defaultName) const
{
	if (!texture)
	{
		return defaultName;
	}

	const ETextureType textureType = AssetManager::GetCachedTextureType(texture->GetTextureHandle());
	std::string textureName = ConvertTextureTypeToTextureName(textureType);

	return textureName.empty() ? defaultName : "material." + textureName + std::to_string(index + 1);
}
