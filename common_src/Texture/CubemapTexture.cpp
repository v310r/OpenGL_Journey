#include "CubemapTexture.h"

#include "glad/glad.h"

#include "stb_image.h"

#include "Texture.h"
#include "Utility/Log.h"
#include "Utility/UtilityFunctions.h"


CubemapTexture::CubemapTexture(const std::string& inCubemapFolder, const CubemapSourceTextureList& inTextureSources)
{
	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);

	uint32_t index = 0;

	stbi_set_flip_vertically_on_load(false);

	m_TextureListSourceData = inTextureSources;
	for (const CubemapSourceTextureOrientationData& textureData : m_TextureListSourceData)
	{
		if (textureData.Orientation == ECubemapTextureOrientation::None)
		{
			LOG_WARN("CubemapTexture: Texture orientation is None for texture: {0}. Skipping this texture.", textureData.TextureName);
			continue;
		}

		STextureConstructionSettings settings;
		settings.bUsedForCubemap = true;
		settings.TextureTarget = UtilityFunctions::ConvertCubemapTextureOrientationToOpenGL(textureData.Orientation);

		const std::string fullPathToFile = inCubemapFolder + "/" + textureData.TextureName;
		std::shared_ptr<Texture> sideTexture = std::make_shared<Texture>(fullPathToFile, settings);
		if (!sideTexture->IsValid())
		{
			LOG_ERROR("CubemapTexture: Failed to load texture: {0}. Skipping this texture.", fullPathToFile);
			continue;
		}

		m_TextureList[index] = sideTexture;
		++index;
	}

	stbi_set_flip_vertically_on_load(true);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

CubemapTexture::CubemapTexture(const SCubemapTextureConstructionSettings& inConstructionSettings)
{
	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);

	if (inConstructionSettings.CubemapType != ECubemapType::ShadowMap)
	{
		LOG_ERROR("Unsupported cubemap type: {0}. This constructor only supports ECubemapType::ShadowMap.", static_cast<int>(inConstructionSettings.CubemapType));
		return;
	}

	m_Width = inConstructionSettings.Width;
	m_Height = inConstructionSettings.Height;

	const uint8_t startIndex = static_cast<uint8_t>(ECubemapTextureOrientation::None) + 1;
	const uint8_t maxNum = static_cast<uint8_t>(ECubemapTextureOrientation::Max);

	for (uint8_t i = startIndex; i < maxNum; ++i)
	{
		const ECubemapTextureOrientation currentOrientation = static_cast<ECubemapTextureOrientation>(i);
		const int32_t currentOrientationGL = UtilityFunctions::ConvertCubemapTextureOrientationToOpenGL(currentOrientation);
		if (currentOrientationGL == -1)
		{
			LOG_ERROR("CubemapTexture: Invalid cubemap texture orientation: {0}. Skipping this orientation.", i);
			continue;
		}

		STextureConstructionSettings settings;
		settings.bUsedForCubemap = true;
		settings.bUsedForShadowMap = true;
		settings.TextureTarget = currentOrientationGL;
		m_TextureList[i - 1] = std::make_shared<Texture>(settings, inConstructionSettings.Width, inConstructionSettings.Height);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

CubemapTexture::~CubemapTexture()
{
	Unbind();
}

void CubemapTexture::Bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);
}

void CubemapTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

uint32_t CubemapTexture::GetId() const
{
	return m_Id;
}

float CubemapTexture::GetWidth() const
{
	return m_Width;
}

float CubemapTexture::GetHeight() const
{
	return m_Height;
}
