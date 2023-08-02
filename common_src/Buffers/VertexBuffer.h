#pragma once


#include "glad/glad.h"


class VertexBuffer
{
public:
	VertexBuffer(float* vertices, size_t size, GLenum usage = GL_STATIC_DRAW);

	~VertexBuffer();

	void Bind();

	void Unbind();

private:
	unsigned int m_Id;
};