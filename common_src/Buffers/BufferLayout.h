#pragma once

#include <vector>
#include <string>
#include "Utility/ShaderUtility.h"


struct BufferAttribute
{
    BufferAttribute(ShaderUtility::ShaderDataType type, const std::string& name, bool isNormalized = false)
        : Type(type), Name(name), bNormalized(isNormalized), SizeInBytes(GetShaderDataTypeSizeInBytes(type)), Offset(0), Count(GetShaderDataTypeCount(type))
    {

    }

    ShaderUtility::ShaderDataType Type;
    std::string Name;
    bool bNormalized;
    uint32_t SizeInBytes;
    uint32_t Offset;
    uint32_t Count;
};

class BufferLayout
{
public:
    BufferLayout() {}

    BufferLayout(const std::initializer_list<BufferAttribute>& attributes);

    uint32_t GetStride() const { return m_Stride; }

    std::vector<BufferAttribute>::iterator begin() { return m_BufferAttributes.begin(); }
    std::vector<BufferAttribute>::iterator end() { return m_BufferAttributes.end(); }
    std::vector<BufferAttribute>::const_iterator begin() const { return m_BufferAttributes.begin(); }
    std::vector<BufferAttribute>::const_iterator end() const { return m_BufferAttributes.end(); }


private:
    void ComputeStrideAndOffset();

    uint32_t m_Stride;
    std::vector<BufferAttribute> m_BufferAttributes;
};