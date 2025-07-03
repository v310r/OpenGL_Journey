#pragma once


#include <cstdint>


class UniformBuffer
{
public:
	UniformBuffer(uint32_t size, uint32_t bindingPoint);
	~UniformBuffer();

	void SetData(const void* data, uint32_t size, uint32_t offset = 0);

protected:
	unsigned int m_Id;
};