#include "BufferLayout.h"

#include "glad/glad.h"

#include <cassert>

#include "Utility/ShaderUtility.h"


using namespace ShaderUtility;

static uint32_t ConvertShaderDataTypeToNumberOfElements(EShaderDataType shaderDataType)
{
	switch (shaderDataType)
	{
		case EShaderDataType::None:		return 0;
		case EShaderDataType::Int:		return 1;
		case EShaderDataType::Int2:		return 2;
		case EShaderDataType::Int3:		return 3;
		case EShaderDataType::Int4:		return 4;
		case EShaderDataType::Float:	return 1;
		case EShaderDataType::Float2:	return 2;
		case EShaderDataType::Float3:	return 3;
		case EShaderDataType::Float4:	return 4;
		case EShaderDataType::Bool:		return 1;
		case EShaderDataType::Mat3:		return 9;
		case EShaderDataType::Mat4:		return 16;
	}

	assert(false);
	return -1;
}

BufferLayout::BufferLayout(const std::initializer_list<BufferAttribute>& attributes)
	: m_BufferAttributes(attributes)
{
	ComputeStrideAndOffset();

	// no support for inter-leaved data
	for (const BufferAttribute& attribute : m_BufferAttributes)
	{
		m_SizeInBytes += attribute.SizeInBytes;
	}
}

size_t BufferLayout::GetAttributesNum() const
{
	return m_BufferAttributes.size();
}

uint32_t BufferLayout::GetNumberOfElementsInAttribute(uint32_t attributeIndex) const
{
	assert(attributeIndex >= 0 && attributeIndex < m_BufferAttributes.size());

	const uint32_t numOfElem = BufferAttribute::GetNumberOfElementsInAttribute(m_BufferAttributes[attributeIndex]);
	return numOfElem;
}

uint32_t BufferLayout::GetAttributeOffset(uint32_t attributeIndex) const
{
	assert(attributeIndex >= 0 && attributeIndex < m_BufferAttributes.size());

	const uint32_t attributeOffset = m_BufferAttributes[attributeIndex].Offset;
	return attributeOffset;
}

size_t BufferLayout::GetTotalSizeInBytes() const
{
	return m_SizeInBytes;
}

void BufferLayout::ComputeStrideAndOffset()
{
	uint32_t offset = 0;
	for (BufferAttribute& attribute : m_BufferAttributes)
	{
		attribute.Offset += offset;
		offset += attribute.SizeInBytes;
	}

	m_Stride = offset;
}

uint32_t BufferAttribute::GetNumberOfElementsInAttribute(const BufferAttribute& attribute)
{
	const uint32_t numOfElem = ConvertShaderDataTypeToNumberOfElements(attribute.Type);
	return numOfElem;
}
