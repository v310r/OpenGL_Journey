#include "Texture.h"

#include "glad/glad.h"

#include "stb_image.h"

#include "Utility/Log.h"


Texture::Texture(const std::string& pathToFile, int32_t textureUnit, bool bHasTransparencyChannel /* = true*/)
{
	m_bHasTransparencyChannel = bHasTransparencyChannel;

	glGenTextures(1, &m_Id);

	glBindTexture(GL_TEXTURE_2D, m_Id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	m_Data = stbi_load(pathToFile.data(), &m_Width, &m_Height, &m_NumColorChannels, 0);
	if (m_Data)
	{
		if (m_bHasTransparencyChannel)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		LOG_INFO("Failed to load texture");
		return;
	}

	m_TextureUnit = textureUnit;
}

Texture::~Texture()
{
	stbi_image_free(m_Data);
	glDeleteTextures(1, &m_Id);
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
