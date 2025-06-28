#include "Framebuffer.h"

#include "glad/glad.h"

#include "Utility/Log.h"


Framebuffer::Framebuffer(int32_t  inWidth, int32_t inHeight)
	: m_Width(inWidth), m_Height(inHeight)
{
	glGenFramebuffers(1, &m_Id);
	Bind();

	constexpr int mipmaplevel = 0;

	glGenTextures(1, &m_ColorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
	glTexImage2D(GL_TEXTURE_2D, mipmaplevel, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, mipmaplevel);

	glGenTextures(1, &m_DepthAndStencilAttachment);
	glBindTexture(GL_TEXTURE_2D, m_DepthAndStencilAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Width, m_Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAndStencilAttachment, mipmaplevel);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	}

	Unbind();
}

Framebuffer::~Framebuffer()
{
	Unbind();

	glDeleteTextures(1, &m_ColorAttachment);
	glDeleteTextures(1, &m_DepthAndStencilAttachment);
	glDeleteFramebuffers(1, &m_Id);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
	glViewport(0, 0, m_Width, m_Height);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int Framebuffer::GetId() const
{
	return m_Id;
}

void Framebuffer::BindColorAttachment()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
}

void Framebuffer::BindDepthAndStencilAttachment()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_DepthAndStencilAttachment);
}

int32_t Framebuffer::GetColorAttachmentUnit() const
{
	return 0; // Color attachment is always bound to texture unit 0
}

int32_t Framebuffer::GetDepthAndStencilAttachmentUnit() const
{
	return 1; // Depth and stencil attachment is always bound to texture unit 1
}

unsigned int Framebuffer::GetColorAttachment() const
{
	return m_ColorAttachment;
}

unsigned int Framebuffer::GetDepthAndStencilAttachment() const
{
	return m_DepthAndStencilAttachment;
}

unsigned int Framebuffer::GetWidth()
{
	return m_Width;
}

unsigned int Framebuffer::GetHeight()
{
	return m_Height;
}

