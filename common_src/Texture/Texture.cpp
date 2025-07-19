#include "Texture.h"

#include "glad/glad.h"

#include "Utility/Log.h"


Texture::Texture(const std::string& pathToFile, int32_t textureUnit, bool bHasTransparencyChannel /* = true*/, const std::string& textureName /* = "Unknown"*/)
{
	std::pair<SAssetHandle, SRawTextureData> outTextureData;
	if (AssetManager::LoadTexture(pathToFile.data(), outTextureData) == false)
	{
		LOG_INFO("Failed to load texture; Path: {0}", pathToFile);
		return;
	}

	const auto& [textureAssetHandle, rawTextureData] = outTextureData;

	m_bHasTransparencyChannel = true;
	if (pathToFile.find(".png") == std::string::npos)
	{
		m_bHasTransparencyChannel = false;
	}

	m_TextureHandle = textureAssetHandle;
	m_Id = rawTextureData.GPUid;

	glBindTexture(GL_TEXTURE_2D, m_Id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	m_Width = rawTextureData.Width;
	m_Height = rawTextureData.Height;
	m_NumColorChannels = rawTextureData.Channels;

	if (AssetManager::CountUsage(textureAssetHandle) == 0)
	{
		if (m_bHasTransparencyChannel)
		{
			if (m_NumColorChannels == 3)
			{
				// If the texture has an alpha channel, we use GL_RGBA
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, rawTextureData.Data);

			}
			else if (m_NumColorChannels == 4)
			{
				// If the texture has an alpha channel, we use GL_RGBA
				// This is the case for PNGs and other formats that support transparency
				// GL_RGBA is used for textures with 4 channels (R, G, B, A)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawTextureData.Data);
			}
		}
		else if (m_NumColorChannels == 1)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, rawTextureData.Data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, rawTextureData.Data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}

	AssetManager::UpdateUsageCounter(textureAssetHandle);

	m_TextureUnit = textureUnit;
}

Texture::Texture(const std::string& pathToFile, const STextureConstructionSettings& inSettings)
{
	std::pair<SAssetHandle, SRawTextureData> outTextureData;
	if (AssetManager::LoadTexture(pathToFile.data(), outTextureData) == false)
	{
		LOG_INFO("Failed to load texture; Path: {0}", pathToFile);
		return;
	}

	const auto& [textureAssetHandle, rawTextureData] = outTextureData;

	m_bHasTransparencyChannel = true;
	if (pathToFile.find(".png") == std::string::npos)
	{
		m_bHasTransparencyChannel = false;
	}

	m_TextureHandle = textureAssetHandle;
	m_Id = rawTextureData.GPUid;

	if (!inSettings.bUsedForCubemap)
	{
		glBindTexture(GL_TEXTURE_2D, m_Id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		// Cubemap should tell OpenGL how it binds texture and its params
	}

	m_Width = rawTextureData.Width;
	m_Height = rawTextureData.Height;
	m_NumColorChannels = rawTextureData.Channels;

	if (AssetManager::CountUsage(textureAssetHandle) == 0)
	{
		if (m_bHasTransparencyChannel)
		{
			if (m_NumColorChannels == 3)
			{
				// If the texture has an alpha channel, we use GL_RGBA
				glTexImage2D(inSettings.TextureTarget, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, rawTextureData.Data);

			}
			else if (m_NumColorChannels == 4)
			{
				// If the texture has an alpha channel, we use GL_RGBA
				// This is the case for PNGs and other formats that support transparency
				// GL_RGBA is used for textures with 4 channels (R, G, B, A)
				glTexImage2D(inSettings.TextureTarget, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawTextureData.Data);
			}
		}
		else if (m_NumColorChannels == 1)
		{
			glTexImage2D(inSettings.TextureTarget, 0, GL_RGB, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, rawTextureData.Data);
		}
		else
		{
			glTexImage2D(inSettings.TextureTarget, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, rawTextureData.Data);
		}

		if (inSettings.TextureTarget == GL_TEXTURE_2D)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	
	AssetManager::UpdateUsageCounter(textureAssetHandle);

	if (inSettings.TextureTarget == GL_TEXTURE_2D)
	{
		m_TextureUnit = inSettings.TextureUnit;
	}
}

Texture::Texture(const STextureConstructionSettings& inSettings, float inWidth, float inHeight)
{
	if (!inSettings.bUsedForShadowMap)
	{
		LOG_ERROR("This constructor should be used only for shadow maps!");
		return;
	}

	// for shadow maps we do not store the texture on our own, but rather delegate it to OpenGL
	m_TextureHandle = {};

	glTexImage2D(inSettings.TextureTarget, 0, GL_DEPTH_COMPONENT, inWidth, inHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
}

Texture::~Texture()
{
	AssetManager::TryDeallocate(m_TextureHandle);
}

bool Texture::IsValid() const
{
	return m_TextureHandle.IsValid();
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_Id);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::ActivateUnit()
{
	glActiveTexture(GL_TEXTURE0 + m_TextureUnit);
}

bool Texture::HasTransparencyChannel() const
{
	return m_bHasTransparencyChannel;
}

void Texture::BindAndActivateUnit()
{
	glActiveTexture(GL_TEXTURE0 + m_TextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_Id);
}

int32_t Texture::GetTextureUnit() const
{
	return m_TextureUnit;
}

SAssetHandle Texture::GetTextureHandle() const
{
	return m_TextureHandle;
}
