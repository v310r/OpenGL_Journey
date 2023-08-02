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
    size_t SizeInBytes;
    size_t Offset;
    size_t Count;
};

class BufferLayout
{
public:
    BufferLayout(const std::initializer_list<BufferAttribute>& attributes);

    size_t GetStride() const { return m_Stride; }

    std::vector<BufferAttribute>::iterator begin() { return m_BufferAttributes.begin(); }
    std::vector<BufferAttribute>::iterator end() { return m_BufferAttributes.end(); }
    std::vector<BufferAttribute>::const_iterator cbegin() const { return m_BufferAttributes.cbegin(); }
    std::vector<BufferAttribute>::const_iterator cend() const { return m_BufferAttributes.cend(); }


private:
    void ComputeStrideAndOffset();

    size_t m_Stride;
    std::vector<BufferAttribute> m_BufferAttributes;
};