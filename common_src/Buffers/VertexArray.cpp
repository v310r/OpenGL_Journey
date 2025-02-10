#include "VertexArray.h"

#include "glad/glad.h"


VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_Id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_Id);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_Id);
	if (HasIndexBuffer())
	{
		m_IndexBuffer->Bind();
	}
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	glBindVertexArray(m_Id);
	vertexBuffer->Bind();

	uint32_t index = 0;
	const BufferLayout& layout = vertexBuffer->GetLayout();
	for (const BufferAttribute& attribute : layout)
	{
		glVertexAttribPointer(index, attribute.Count, ConvertShaderDataTypeToOpenGLType(attribute.Type), attribute.bNormalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)attribute.Offset);
		glEnableVertexAttribArray(index);
		++index;
	}

	m_VertexBuffers.emplace_back(vertexBuffer);
	m_VertexCount += vertexBuffer->GetVertexCount();
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	m_IndexBuffer = indexBuffer;
}

uint32_t VertexArray::GetVertexCount() const
{
	return m_VertexCount;
}

bool VertexArray::HasIndexBuffer() const
{
	return m_IndexBuffer.get() != nullptr;
}

uint32_t VertexArray::GetIndexBufferCount() const
{
	return m_IndexBuffer->GetDataCount();
}

