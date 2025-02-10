#include "ShaderUtility.h"

#include <cassert>


unsigned int ShaderUtility::GetShaderDataTypeSizeInBytes(ShaderUtility::EShaderDataType type)
{
    switch (type)
    {
        case ShaderUtility::EShaderDataType::Float:	    return 4;
        case ShaderUtility::EShaderDataType::Float2:	    return 4 * 2;
        case ShaderUtility::EShaderDataType::Float3:	    return 4 * 3;
        case ShaderUtility::EShaderDataType::Float4:	    return 4 * 4;
        case ShaderUtility::EShaderDataType::Int:	    return 4;
        case ShaderUtility::EShaderDataType::Int2:	    return 4 * 2;
        case ShaderUtility::EShaderDataType::Int3:	    return 4 * 3;
        case ShaderUtility::EShaderDataType::Int4:	    return 4 * 4;
        case ShaderUtility::EShaderDataType::Mat3:	    return 4 * 3 * 3;
        case ShaderUtility::EShaderDataType::Mat4:	    return 4 * 4 * 4;
        case ShaderUtility::EShaderDataType::Bool:	    return 1;

        default:                        assert(false && "Unknown type"); return 0;
    }
}

unsigned int ShaderUtility::GetShaderDataTypeCount(EShaderDataType type)
{
    switch (type)
    {
        case ShaderUtility::EShaderDataType::Float:	    return 1;
        case ShaderUtility::EShaderDataType::Float2:	    return 2;
        case ShaderUtility::EShaderDataType::Float3:	    return 3;
        case ShaderUtility::EShaderDataType::Float4:	    return 4;
        case ShaderUtility::EShaderDataType::Int:	    return 1;
        case ShaderUtility::EShaderDataType::Int2:	    return 2;
        case ShaderUtility::EShaderDataType::Int3:	    return 3;
        case ShaderUtility::EShaderDataType::Int4:	    return 4;
        case ShaderUtility::EShaderDataType::Mat3:	    return 3 * 3;
        case ShaderUtility::EShaderDataType::Mat4:	    return 4 * 4;
        case ShaderUtility::EShaderDataType::Bool:	    return 1;

        default:                        assert(false && "Unknown type"); return 0;
    }
}

GLenum ShaderUtility::ConvertShaderDataTypeToOpenGLType(EShaderDataType type)
{
    switch (type)
    {
        case ShaderUtility::EShaderDataType::Float:	    return GL_FLOAT;
        case ShaderUtility::EShaderDataType::Float2:	    return GL_FLOAT;
        case ShaderUtility::EShaderDataType::Float3:	    return GL_FLOAT;
        case ShaderUtility::EShaderDataType::Float4:	    return GL_FLOAT;
        case ShaderUtility::EShaderDataType::Int:	    return GL_INT;
        case ShaderUtility::EShaderDataType::Int2:	    return GL_INT;
        case ShaderUtility::EShaderDataType::Int3:	    return GL_INT;
        case ShaderUtility::EShaderDataType::Int4:	    return GL_INT;
        case ShaderUtility::EShaderDataType::Mat3:	    return GL_FLOAT;
        case ShaderUtility::EShaderDataType::Mat4:	    return GL_FLOAT;
        case ShaderUtility::EShaderDataType::Bool:	    return GL_BOOL;

        default:       assert(false && "Unknown type"); return 0;
    }
}
