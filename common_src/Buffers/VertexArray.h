#pragma once

#include <vector>
#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"


class VertexArray
{
public:
	VertexArray();

	~VertexArray();

	void Bind();
	
	void Unbind();

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);

	void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	uint32_t GetVertexCount() const;

	bool HasIndexBuffer() const;

	uint32_t GetIndexBufferCount() const;

	unsigned int GetId() const { return m_Id; }

private:
	unsigned int m_Id;
	// we can use each buffer for an individual attribute if we want to 
	// (but usually only 1 VBO will be used for current VAO [interleaved attributes])
	std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;

	uint32_t m_VertexCount = 0;
};