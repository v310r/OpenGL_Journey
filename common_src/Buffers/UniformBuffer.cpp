#include "UniformBuffer.h"

#include "glad/glad.h"


UniformBuffer::UniformBuffer(uint32_t size, uint32_t bindingPoint)
{
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_UNIFORM_BUFFER, m_Id);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_Id);
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_Id);
}

void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset /*= 0*/)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_Id);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}
