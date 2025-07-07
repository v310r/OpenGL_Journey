#include "ShaderUtility.h"

#include <cassert>

#include "Shader1/Shader.h"


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

EShaderType ShaderUtility::ConvertShaderPathToShaderType(const std::string& shaderPath)
{
	if (shaderPath.find(".vert") != std::string::npos)
	{
		return EShaderType::VertexShader;
	}
	else if (shaderPath.find(".frag") != std::string::npos)
	{
		return EShaderType::FragmentShader;
	}
	else if (shaderPath.find(".geom") != std::string::npos)
	{
		return EShaderType::GeometryShader;
	}

	assert(false && "Unknown shader type");
	return EShaderType::None;
}

uint32_t ShaderUtility::ConvertShaderTypeToOpenGLType(EShaderType type)
{
	switch (type)
	{
		case EShaderType::VertexShader:		return GL_VERTEX_SHADER;
		case EShaderType::FragmentShader:	return GL_FRAGMENT_SHADER;
		case EShaderType::GeometryShader:	return GL_GEOMETRY_SHADER;

		default: assert(false && "Unknown shader type"); return 0;
	}
}

uint32_t ShaderUtility::ConvertShaderPathToOpenGLType(const std::string& shaderPath)
{
	const EShaderType shaderType = ConvertShaderPathToShaderType(shaderPath);
	return ConvertShaderTypeToOpenGLType(shaderType);
}
