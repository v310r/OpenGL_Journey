#pragma once


#include <cstdint>


class Framebuffer
{
public:
	Framebuffer(int32_t  inWidth, int32_t inHeight);
	~Framebuffer();

	void Bind();
	void Unbind();

	unsigned int GetId() const;

	void BindColorAttachment();
	void BindDepthAndStencilAttachment();

	int32_t GetColorAttachmentUnit() const;
	int32_t GetDepthAndStencilAttachmentUnit() const;

	unsigned int GetColorAttachment() const;
	unsigned int GetDepthAndStencilAttachment() const;

	unsigned int GetWidth();
	unsigned int GetHeight();

protected:
	unsigned int m_Id;

	unsigned int m_ColorAttachment = 0;

	// combined into one texture
	unsigned int m_DepthAndStencilAttachment = 0;

	int32_t m_Width = 0, m_Height = 0;
};