#include "IndexBuffer.h"

#include "glad/glad.h"


IndexBuffer::IndexBuffer(unsigned int* indices, uint32_t count)
{
	m_Data = indices;
	m_DataCount = count;
	m_DataSizeInBytes = count * sizeof(unsigned int);

	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_DataSizeInBytes, indices, GL_STATIC_DRAW);

}

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::GetId() const
{
	return m_Id;
}

unsigned int* IndexBuffer::GetData() const
{
	return m_Data;
}

uint32_t IndexBuffer::GetDataCount() const
{
	return m_DataCount;
}

uint32_t IndexBuffer::GetDataSizeInBytes() const
{
	return m_DataSizeInBytes;
}

