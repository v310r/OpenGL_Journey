#include "Framebuffer2.h"

#include "glad/glad.h"

#include "Texture/CubemapTexture.h"
#include "Utility/Log.h"


static const uint32_t s_MaxFramebufferSize = 8192;

namespace Utils
{

	static GLenum TextureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
	{
		glCreateTextures(TextureTarget(multisampled), count, outID);
	}

	static void BindTexture(bool multisampled, uint32_t id)
	{
		glBindTexture(TextureTarget(multisampled), id);
	}

	static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
	}

	static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
	}

	static bool IsDepthFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
			case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
		}

		return false;
	}

	static GLenum FBTextureFormatToGL(FramebufferTextureFormat format)
	{
		switch (format)
		{
			case FramebufferTextureFormat::RGBA8:			return GL_RGBA8;
			case FramebufferTextureFormat::RED_INTEGER:		return GL_RED_INTEGER;
			case FramebufferTextureFormat::RGB8:			return GL_RGB8;
			case FramebufferTextureFormat::DEPTH24STENCIL8:	return GL_DEPTH24_STENCIL8;
			case FramebufferTextureFormat::RGBA16F:			return GL_RGBA16F;
		}

		return 0;
	}

}

Framebuffer2::Framebuffer2(const FramebufferSpecification& spec)
	: m_Specification(spec)
{
	for (auto spec : m_Specification.Attachments.Attachments)
	{
		if (!Utils::IsDepthFormat(spec.TextureFormat))
		{
			m_ColorAttachmentSpecifications.emplace_back(spec);
		}
		else
		{
			m_DepthAttachmentSpecification = spec;
		}
	}

	Invalidate();
}

Framebuffer2::Framebuffer2(const std::shared_ptr<CubemapTexture>& inCubemapTexture)
{
	glCreateFramebuffers(1, &m_Id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Id);

	m_CubemapTextureId = inCubemapTexture->GetId();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_CubemapTextureId, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	m_Specification.Width = inCubemapTexture->GetWidth();
	m_Specification.Height = inCubemapTexture->GetHeight();

	m_Specification.Attachments = {FramebufferTextureFormat::DEPTH24STENCIL8 };


	bUsingCubemapTexture = true;

	Unbind();
}

Framebuffer2::~Framebuffer2()
{
	glDeleteFramebuffers(1, &m_Id);
	glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
	glDeleteTextures(1, &m_DepthAttachment);
}

void Framebuffer2::Invalidate()
{
	if (m_Id)
	{
		glDeleteFramebuffers(1, &m_Id);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);

		m_ColorAttachments.clear();
		m_DepthAttachment = 0;
	}

	glCreateFramebuffers(1, &m_Id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Id);

	bool multisample = m_Specification.Samples > 1;

	// Attachments
	if (m_ColorAttachmentSpecifications.size())
	{
		m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
		Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

		for (size_t i = 0; i < m_ColorAttachments.size(); i++)
		{
			Utils::BindTexture(multisample, m_ColorAttachments[i]);
			switch (m_ColorAttachmentSpecifications[i].TextureFormat)
			{
				case FramebufferTextureFormat::RGB8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGB8, GL_RGB, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormat::RGBA16F:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA16F, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
			}
		}
	}

	if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
	{
		Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
		Utils::BindTexture(multisample, m_DepthAttachment);
		switch (m_DepthAttachmentSpecification.TextureFormat)
		{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
		}
	}

	if (m_ColorAttachments.size() > 1)
	{
		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(m_ColorAttachments.size(), buffers);
	}
	else if (m_ColorAttachments.empty())
	{
		// Only depth-pass
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	}

	Unbind();
}

uint32_t Framebuffer2::GetId() const
{
	return m_Id;
}

void Framebuffer2::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
	glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

void Framebuffer2::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer2::Resize(uint32_t width, uint32_t height)
{
	if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
	{
		return;
	}

	m_Specification.Width = width;
	m_Specification.Height = height;

	Invalidate();
}

int Framebuffer2::ReadPixel(uint32_t attachmentIndex, int x, int y)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	int pixelData;
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

	return pixelData;
}

void Framebuffer2::ClearAttachment(uint32_t attachmentIndex, int value)
{
	auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
	glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::FBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
}

uint32_t Framebuffer2::GetColorAttachmentId(uint32_t index /*= 0*/) const
{
	return m_ColorAttachments[index];
}

uint32_t Framebuffer2::GetDepthAttachmentId() const
{
	return m_DepthAttachment;
}

uint32_t Framebuffer2::GetColorAttachmentTextureUnit(uint32_t index /*= 0*/) const
{
	return 31 - index;
}

uint32_t Framebuffer2::GetDepthAttachmentTextureUnit() const
{
	return 31 - m_ColorAttachments.size();
}

void Framebuffer2::BindColorAttachment(uint32_t index /*= 0*/) const
{
	glActiveTexture(GL_TEXTURE31 - index);
	glBindTexture(Utils::TextureTarget(m_Specification.Samples > 1), m_ColorAttachments[index]);
}

void Framebuffer2::BindColorAttachment(uint32_t index /*= 0*/, uint32_t activeTexture) const
{
	glActiveTexture(GL_TEXTURE0 + activeTexture);
	glBindTexture(Utils::TextureTarget(m_Specification.Samples > 1), m_ColorAttachments[index]);
}

void Framebuffer2::BindDepthAttachment() const
{
	if (bUsingCubemapTexture)
	{
		glActiveTexture(GL_TEXTURE31 - m_ColorAttachments.size());
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTextureId);
	}
	else
	{
		glActiveTexture(GL_TEXTURE31 - m_ColorAttachments.size());
		glBindTexture(Utils::TextureTarget(m_Specification.Samples > 1), m_DepthAttachment);
	}
}

const FramebufferSpecification& Framebuffer2::GetSpecification() const
{
	return m_Specification;
}
