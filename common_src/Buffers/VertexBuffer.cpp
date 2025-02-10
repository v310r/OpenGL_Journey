#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(float* vertices, size_t count, GLenum usage)
{
	m_Data = vertices;
	m_DataCount = (uint32_t)count;
	m_DataSizeInBytes = count * sizeof(float);

	glCreateBuffers(1, &m_Id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ARRAY_BUFFER, m_DataSizeInBytes, vertices, usage);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_Id);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetLayout(const BufferLayout& layout)
{
	m_Layout = layout;

	m_VertexCount = GetDataSizeInBytes() / m_Layout.GetTotalSizeInBytes();
}

uint32_t VertexBuffer::GetId() const
{
	return m_Id;
}

uint32_t VertexBuffer::GetDataOffset() const
{
	return 0;
}

uint32_t VertexBuffer::GetVertexStride() const
{
	const uint32_t vertexStride = m_Layout.GetStride();
	return vertexStride;
}

uint64_t VertexBuffer::GetLayoutAttributesNum() const
{
	return m_Layout.GetAttributesNum();
}

uint32_t VertexBuffer::GetNumberOfElementsInAttribute(uint32_t attributeIndex) const
{
	const uint32_t numOfElem = m_Layout.GetNumberOfElementsInAttribute(attributeIndex);
	return numOfElem;
}

uint32_t VertexBuffer::GetAttributeOffset(uint32_t attributeIndex) const
{
	const uint32_t attributeOffset = m_Layout.GetAttributeOffset(attributeIndex);
	return attributeOffset;
}

float* VertexBuffer::GetData() const
{
	return m_Data;
}

uint32_t VertexBuffer::GetDataCount() const
{
	return m_DataCount;
}

uint32_t VertexBuffer::GetVertexCount() const
{
	return m_VertexCount;
}

uint32_t VertexBuffer::GetDataSizeInBytes() const
{
	return m_DataSizeInBytes;
}
