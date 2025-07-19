#pragma once


#include <array>
#include <string>
#include <initializer_list>

#include "Texture.h"


enum class ECubemapType : uint8_t 
{
	None = 0,
	Skybox,
	ShadowMap
};

enum class ECubemapTextureOrientation : uint8_t
{
	None = 0,
	Right = 1,
	Left = 2,
	Top = 3,
	Bottom = 4,
	Back = 5,
	Front = 6,
	Max
};

struct CubemapSourceTextureOrientationData
{
	std::string TextureName;

	ECubemapTextureOrientation Orientation;
};

using CubemapSourceTextureList = std::array<CubemapSourceTextureOrientationData, 6>;

using CubemapTextureList = std::array<std::shared_ptr<Texture>, 6>;

struct SCubemapTextureConstructionSettings
{
	ECubemapType CubemapType = ECubemapType::None;
	
	// optional width and height, usually used for shadow maps
	float Width = 1024.0f, Height = 1024.0f;
};


class CubemapTexture
{
	
public:
	CubemapTexture(const std::string& inCubemapFolder, const CubemapSourceTextureList& inTextureSources);

	CubemapTexture(const SCubemapTextureConstructionSettings& inConstructionSettings);

	~CubemapTexture();

	void Bind();

	void Unbind();

	uint32_t GetId() const;

	float GetWidth() const;
	float GetHeight() const;

protected:
	CubemapSourceTextureList m_TextureListSourceData;

	// This is the actual textures that are loaded into OpenGL and memory
	CubemapTextureList m_TextureList;

	unsigned int m_Id = 0;

	float m_Width = 1024.0f, m_Height;
};