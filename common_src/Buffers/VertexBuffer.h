#pragma once

#include "glad/glad.h"
#include "BufferLayout.h"


class VertexBuffer
{
public:
	VertexBuffer() {}

	VertexBuffer(float* vertices, size_t count, GLenum usage = GL_STATIC_DRAW);

	~VertexBuffer();

	void Bind();

	void Unbind();

	const BufferLayout& GetLayout() const { return m_Layout; }
	void SetLayout(const BufferLayout& layout);

	uint32_t GetId() const;

	uint32_t GetDataOffset() const;

	uint32_t GetVertexStride() const;

	uint64_t GetLayoutAttributesNum() const;

	uint32_t GetNumberOfElementsInAttribute(uint32_t attributeIndex) const;

	// relative offset (relative to other attributes)
	uint32_t GetAttributeOffset(uint32_t attributeIndex) const;

	float* GetData() const;
	uint32_t GetDataCount() const;
	uint32_t GetVertexCount() const;
	uint32_t GetDataSizeInBytes() const;

protected:
	BufferLayout m_Layout;

	float* m_Data = nullptr;
	uint32_t m_DataCount = 0;
	uint32_t m_VertexCount = 0;
	uint32_t m_DataSizeInBytes = 0;

	uint32_t m_Id = 0;
};