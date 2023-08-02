#pragma once


class IndexBuffer
{
public:
	IndexBuffer();

	void Bind();

	void Unbind();

private:
	unsigned int m_Id;
};