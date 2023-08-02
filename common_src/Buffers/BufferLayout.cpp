#include "BufferLayout.h"

#include "glad/glad.h"


BufferLayout::BufferLayout(const std::initializer_list<BufferAttribute>& attributes)
    : m_BufferAttributes(attributes)
{
    ComputeStrideAndOffset();
}

void BufferLayout::ComputeStrideAndOffset()
{
    size_t offset = 0;
    for (BufferAttribute& attribute : m_BufferAttributes)
    {
        attribute.Offset += offset;
        offset += attribute.SizeInBytes;
    }

    m_Stride = offset;
}
