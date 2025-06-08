#pragma once

#include <string>

#include "AssetManager/AssetManager.h"


class Texture
{
public:
	Texture(const std::string& pathToFile, int32_t textureUnit, bool bHasTransparencyChannel = true, const std::string& textureName = "Unknown");

	~Texture();

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