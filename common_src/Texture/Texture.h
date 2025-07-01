#pragma once

#include <string>

#include "AssetManager/AssetManager.h"


struct STextureConstructionSettings
{
	std::string TextureName = "Unknown";

	int32_t TextureUnit = -1;

	int32_t TextureTarget = 0; // e.g. GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, etc. for Y and Z

	bool bUsedForCubemap = false;
};

class Texture
{
public:
	// bHasTransparencyChannel is useless but left here for compatibility
	Texture(const std::string& pathToFile, int32_t textureUnit, bool bHasTransparencyChannel = true, const std::string& textureName = "Unknown");

	Texture(const std::string& pathToFile, const STextureConstructionSettings& inSettings);

	~Texture();

	bool IsValid() const;

	void Bind();

	void Unbind();

	void ActivateUnit();

	bool HasTransparencyChannel() const;

	void BindAndActivateUnit();

	int32_t GetTextureUnit() const;

	SAssetHandle GetTextureHandle() const;

protected:
	unsigned int m_Id;

	int m_Width = 0, m_Height = 0;

	int m_NumColorChannels = 0;

	int32_t m_TextureUnit = -1;

	bool m_bHasTransparencyChannel = true;

	SAssetHandle m_TextureHandle;
};