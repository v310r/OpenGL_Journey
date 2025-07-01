#pragma once


#include <array>
#include <string>
#include <initializer_list>

#include "Texture.h"


enum class CubemapTextureOrientation :uint8_t
{
	None = 0,
	Right = 1,
	Left = 2,
	Top = 3,
	Bottom = 4,
	Back = 5,
	Front = 6
};

struct CubemapSourceTextureOrientationData
{
	std::string TextureName;

	CubemapTextureOrientation Orientation;
};

using CubemapSourceTextureList = std::array<CubemapSourceTextureOrientationData, 6>;

using CubemapTextureList = std::array<std::shared_ptr<Texture>, 6>;


class CubemapTexture
{
	
public:
	CubemapTexture(const std::string& inCubemapFolder, const CubemapSourceTextureList& inTextureSources);

	~CubemapTexture();

	void Bind();

	void Unbind();

protected:
	CubemapSourceTextureList m_TextureListSourceData;

	// This is the actual textures that are loaded into OpenGL and memory
	CubemapTextureList m_TextureList;

	unsigned int m_Id = 0;
};