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
		if (textureData.Orientation == CubemapTextureOrientation::None)
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

