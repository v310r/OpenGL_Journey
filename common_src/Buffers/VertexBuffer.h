#pragma once

#include "glad/glad.h"
#include "BufferLayout.h"


class VertexBuffer
{
public:
	VertexBuffer() {}

	VertexBuffer(float* vertices, size_t size, GLenum usage = GL_STATIC_DRAW);

	~VertexBuffer();

	void Bind();

	void Unbind();

	const BufferLayout& GetLayout() const { return m_Layout; }
	void SetLayout(const BufferLayout& layout) { m_Layout = layout; };

private:
	unsigned int m_Id;
	BufferLayout m_Layout;
};