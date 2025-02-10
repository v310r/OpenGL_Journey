#pragma once

#include <vector>
#include <string>

#include "Utility/ShaderUtility.h"


struct BufferAttribute
{
	BufferAttribute(ShaderUtility::EShaderDataType type, const std::string& name, bool isNormalized = false)
		: Type(type), Name(name), bNormalized(isNormalized), SizeInBytes(GetShaderDataTypeSizeInBytes(type)), Offset(0), Count(GetShaderDataTypeCount(type))
	{

	}

	ShaderUtility::EShaderDataType Type;
	std::string Name;
	bool bNormalized;
	uint32_t SizeInBytes;
	uint32_t Offset;
	uint32_t Count;

	static uint32_t GetNumberOfElementsInAttribute(const BufferAttribute& attribute);
};

class BufferLayout
{
public:
	BufferLayout()
	{
	}

	BufferLayout(const std::initializer_list<BufferAttribute>& attributes);

	uint32_t GetStride() const
	{
		return m_Stride;
	}

	size_t GetAttributesNum() const;

	uint32_t GetNumberOfElementsInAttribute(uint32_t attributeIndex) const;

	uint32_t GetAttributeOffset(uint32_t attributeIndex) const;

	size_t GetTotalSizeInBytes() const;

	std::vector<BufferAttribute>::iterator begin()
	{
		return m_BufferAttributes.begin();
	}

	std::vector<BufferAttribute>::iterator end()
	{
		return m_BufferAttributes.end();
	}

	std::vector<BufferAttribute>::const_iterator begin() const
	{
		return m_BufferAttributes.begin();
	}

	std::vector<BufferAttribute>::const_iterator end() const
	{
		return m_BufferAttributes.end();
	}


private:
	void ComputeStrideAndOffset();

	uint32_t m_Stride;
	uint32_t m_SizeInBytes = 0;
	std::vector<BufferAttribute> m_BufferAttributes;
};