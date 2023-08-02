#pragma once

class VertexArray
{
public:
	VertexArray();

	void Bind();

	void Unbind();

private:
	unsigned int m_Id;
};