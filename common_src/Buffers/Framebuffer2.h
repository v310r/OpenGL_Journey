#pragma once

#include <cstdint>
#include <vector>
#include <memory>


class CubemapTexture;

enum class FramebufferTextureFormat
{
	None = 0,

	// Color
	RGBA8,
	RGB8,
	RED_INTEGER,

	// Depth/stencil
	DEPTH24STENCIL8,

	// Defaults
	Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification
{
	FramebufferTextureSpecification() = default;
	FramebufferTextureSpecification(FramebufferTextureFormat format)
		: TextureFormat(format)
	{
	}

	FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
};

struct FramebufferAttachmentSpecification
{
	FramebufferAttachmentSpecification() = default;
	FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
		: Attachments(attachments)
	{
	}

	std::vector<FramebufferTextureSpecification> Attachments;
};

struct FramebufferSpecification
{
	uint32_t Width = 0, Height = 0;
	FramebufferAttachmentSpecification Attachments;
	uint32_t Samples = 1;
};

class Framebuffer2 
{
public:
	Framebuffer2(const FramebufferSpecification& spec);

	// assume we use it for shadow mapping
	Framebuffer2(const std::shared_ptr<CubemapTexture>& inCubemapTexture);

	~Framebuffer2();

	void Invalidate();

	uint32_t GetId() const;

	void Bind();
	void Unbind();

	void Resize(uint32_t width, uint32_t height);
	int ReadPixel(uint32_t attachmentIndex, int x, int y);

	void ClearAttachment(uint32_t attachmentIndex, int value);

	uint32_t GetColorAttachmentId(uint32_t index = 0) const;
	uint32_t GetDepthAttachmentId() const;

	uint32_t GetColorAttachmentTextureUnit(uint32_t index = 0) const;
	uint32_t GetDepthAttachmentTextureUnit() const;

	void BindColorAttachment(uint32_t index = 0) const;
	void BindDepthAttachment() const;

	const FramebufferSpecification& GetSpecification() const;

private:
	FramebufferSpecification m_Specification;

	std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
	FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

	std::vector<uint32_t> m_ColorAttachments;
	uint32_t m_DepthAttachment = 0;

	uint32_t m_Id = 0;

	uint32_t m_CubemapTextureId = 0; // used only if we use cubemap texture for shadow mapping

	bool bUsingCubemapTexture = false;
};