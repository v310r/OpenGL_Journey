#pragma once

#include <string>


class Texture
{
public:
	Texture(const std::string& pathToFile, int32_t textureUnit, bool bHasTransparencyChannel = true);

	~Texture();

	void Bind();

	void Unbind();

	void ActivateUnit();

	bool HasTransparencyChannel() const;

	void BindAndActivateUnit();

	int32_t GetTextureUnit() const;

protected:
	unsigned int m_Id;

	unsigned char* m_Data = nullptr;

	int m_Width = 0, m_Height = 0;

	int m_NumColorChannels = 0;

	int32_t m_TextureUnit = -1;

	bool m_bHasTransparencyChannel = true;
};