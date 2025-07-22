#include "Material.h"

#include "Shader1/Shader.h"
#include "Texture/Texture.h"


static std::string_view s_AmbientName = "material.ambient";
static std::string_view s_DiffuseName = "material.diffuse";
static std::string_view s_SpecularName = "material.specular";
static std::string_view s_NormalName = "material.normal";
static std::string_view s_EmissionName = "material.emission";
static std::string_view s_ShininessName = "material.shininess";

static std::string_view GetAmbientName()
{
	return s_AmbientName;
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
	return s_NormalName;
}

static std::string_view GetEmissionName()
{
	return s_EmissionName;
}

static std::string_view GetShininessName()
{
	return s_ShininessName;
}

Material::Material(const std::shared_ptr<Shader>& shader)
{
	SetShader(shader);
}

void Material::Bind()
{
	m_ShaderRef->Bind();

	if (m_DiffuseTexture.get())
	{
		m_ShaderRef->SetInt(GetDiffuseName(), m_DiffuseTexture->GetTextureUnit());
		m_DiffuseTexture->BindAndActivateUnit();
	}

	if (m_SpecularTexture.get())
	{
		m_ShaderRef->SetInt(GetSpecularName(), m_SpecularTexture->GetTextureUnit());
		m_SpecularTexture->BindAndActivateUnit();
	}

	if (m_NormalTexture.get())
	{
		m_ShaderRef->SetInt(GetNormalName(), m_NormalTexture->GetTextureUnit());
		m_NormalTexture->BindAndActivateUnit();
	}

	if (m_EmissionTexture.get())
	{
		m_ShaderRef->SetInt(GetEmissionName(), m_EmissionTexture->GetTextureUnit());
		m_EmissionTexture->BindAndActivateUnit();
	}

	m_ShaderRef->SetFloat(GetShininessName(), m_Shininess);
}

void Material::Unbind()
{
	m_ShaderRef->Unbind();
}

const std::shared_ptr<Texture>& Material::GetDiffuse() const
{
	return m_DiffuseTexture;
}

const std::shared_ptr<Texture>& Material::GetSpecular()
{
	return m_SpecularTexture;
}

float& Material::GetShininess()
{
	return m_Shininess;
}

void Material::SetDiffuse(const std::shared_ptr<Texture>& texture)
{
	m_DiffuseTexture = texture;
}

void Material::SetSpecular(const std::shared_ptr<Texture>& texture)
{
	m_SpecularTexture = texture;
}

void Material::SetNormal(const std::shared_ptr<Texture>& texture)
{
	m_NormalTexture = texture;
}

void Material::SetEmission(const std::shared_ptr<Texture>& texture)
{
	m_EmissionTexture = texture;
}

void Material::SetShininess(float value)
{
	m_Shininess = value;
}

void Material::SetShader(const std::shared_ptr<Shader>& shader)
{
	m_ShaderRef = shader;
}


const std::shared_ptr<Shader>& Material::GetShader() const
{
	return m_ShaderRef;
}

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////


SimpleMaterial::SimpleMaterial(const std::shared_ptr<Shader>& shader)
{
	SetShader(shader);
}

void SimpleMaterial::Bind()
{
	m_ShaderRef->Bind();

	m_ShaderRef->SetFloat3(GetAmbientName(), m_Ambient);
	m_ShaderRef->SetFloat3(GetDiffuseName(), m_Diffuse);
	m_ShaderRef->SetFloat3(GetSpecularName(), m_Specular);
	m_ShaderRef->SetFloat(GetShininessName(), m_Shininess);
}

void SimpleMaterial::Unbind()
{
	m_ShaderRef->Unbind();
}

glm::vec3& SimpleMaterial::GetAmbient()
{
	return m_Ambient;
}

glm::vec3& SimpleMaterial::GetDiffuse()
{
	return m_Diffuse;
}

glm::vec3& SimpleMaterial::GetSpecular()
{
	return m_Specular;
}

float& SimpleMaterial::GetShininess()
{
	return m_Shininess;
}

void SimpleMaterial::SetAmbient(glm::vec3 value)
{
	m_Ambient = value;
}

void SimpleMaterial::SetDiffuse(glm::vec3 value)
{
	m_Diffuse = value;
}

void SimpleMaterial::SetSpecular(glm::vec3 value)
{
	m_Specular = value;
}

void SimpleMaterial::SetShininess(float value)
{
	m_Shininess = value;
}

void SimpleMaterial::SetShader(const std::shared_ptr<Shader>& shader)
{
	m_ShaderRef = shader;
}

const std::shared_ptr<Shader>& SimpleMaterial::GetShader() const
{
	return m_ShaderRef;
}
