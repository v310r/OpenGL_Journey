#pragma once

#include "glad/glad.h"


namespace ShaderUtility
{
    enum class ShaderDataType
    {
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
        Mat3,
        Mat4,
        Bool
    };

    size_t GetShaderDataTypeSizeInBytes(ShaderDataType type);

    size_t GetShaderDataTypeCount(ShaderDataType type);

    GLenum ConvertShaderDataTypeToOpenGLType(ShaderDataType type);
}