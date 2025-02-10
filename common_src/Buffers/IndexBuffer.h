#pragma once


#include <stdint.h>


class IndexBuffer
{
public:
	IndexBuffer(unsigned int* indices, uint32_t count);

	void Bind();

	void Unbind();

	unsigned int GetId() const;

	unsigned int* GetData() const;
	uint32_t GetDataCount() const;
	uint32_t GetDataSizeInBytes() const;

private:
	unsigned int m_Id;

	unsigned int* m_Data = nullptr;

	uint32_t m_DataCount = 0;
	uint32_t m_DataSizeInBytes = 0;
};