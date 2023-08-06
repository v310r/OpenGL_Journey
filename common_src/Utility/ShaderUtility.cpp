#include "ShaderUtility.h"

#include <cassert>


unsigned int ShaderUtility::GetShaderDataTypeSizeInBytes(ShaderUtility::ShaderDataType type)
{
    switch (type)
    {
        case ShaderUtility::ShaderDataType::Float:	    return 4;
        case ShaderUtility::ShaderDataType::Float2:	    return 4 * 2;
        case ShaderUtility::ShaderDataType::Float3:	    return 4 * 3;
        case ShaderUtility::ShaderDataType::Float4:	    return 4 * 4;
        case ShaderUtility::ShaderDataType::Int:	    return 4;
        case ShaderUtility::ShaderDataType::Int2:	    return 4 * 2;
        case ShaderUtility::ShaderDataType::Int3:	    return 4 * 3;
        case ShaderUtility::ShaderDataType::Int4:	    return 4 * 4;
        case ShaderUtility::ShaderDataType::Mat3:	    return 4 * 3 * 3;
        case ShaderUtility::ShaderDataType::Mat4:	    return 4 * 4 * 4;
        case ShaderUtility::ShaderDataType::Bool:	    return 1;

        default:                        assert(false && "Unknown type"); return 0;
    }
}

unsigned int ShaderUtility::GetShaderDataTypeCount(ShaderDataType type)
{
    switch (type)
    {
        case ShaderUtility::ShaderDataType::Float:	    return 1;
        case ShaderUtility::ShaderDataType::Float2:	    return 2;
        case ShaderUtility::ShaderDataType::Float3:	    return 3;
        case ShaderUtility::ShaderDataType::Float4:	    return 4;
        case ShaderUtility::ShaderDataType::Int:	    return 1;
        case ShaderUtility::ShaderDataType::Int2:	    return 2;
        case ShaderUtility::ShaderDataType::Int3:	    return 3;
        case ShaderUtility::ShaderDataType::Int4:	    return 4;
        case ShaderUtility::ShaderDataType::Mat3:	    return 3 * 3;
        case ShaderUtility::ShaderDataType::Mat4:	    return 4 * 4;
        case ShaderUtility::ShaderDataType::Bool:	    return 1;

        default:                        assert(false && "Unknown type"); return 0;
    }
}

GLenum ShaderUtility::ConvertShaderDataTypeToOpenGLType(ShaderDataType type)
{
    switch (type)
    {
        case ShaderUtility::ShaderDataType::Float:	    return GL_FLOAT;
        case ShaderUtility::ShaderDataType::Float2:	    return GL_FLOAT;
        case ShaderUtility::ShaderDataType::Float3:	    return GL_FLOAT;
        case ShaderUtility::ShaderDataType::Float4:	    return GL_FLOAT;
        case ShaderUtility::ShaderDataType::Int:	    return GL_INT;
        case ShaderUtility::ShaderDataType::Int2:	    return GL_INT;
        case ShaderUtility::ShaderDataType::Int3:	    return GL_INT;
        case ShaderUtility::ShaderDataType::Int4:	    return GL_INT;
        case ShaderUtility::ShaderDataType::Mat3:	    return GL_FLOAT;
        case ShaderUtility::ShaderDataType::Mat4:	    return GL_FLOAT;
        case ShaderUtility::ShaderDataType::Bool:	    return GL_BOOL;

        default:       assert(false && "Unknown type"); return 0;
    }
}
