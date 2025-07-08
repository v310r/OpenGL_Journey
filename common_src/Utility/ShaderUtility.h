#pragma once

#include "glad/glad.h"

#include <string>
#include <cstdint>

enum class EShaderType : uint8_t;

namespace ShaderUtility
{
	enum class EShaderDataType : uint8_t
	{
		None = 0,
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

	unsigned int GetShaderDataTypeSizeInBytes(EShaderDataType type);

	unsigned int GetShaderDataTypeCount(EShaderDataType type);

	GLenum ConvertShaderDataTypeToOpenGLType(EShaderDataType type);

	EShaderType ConvertShaderPathToShaderType(const std::string& shaderPath);

	uint32_t ConvertShaderTypeToOpenGLType(EShaderType type);

	uint32_t ConvertShaderPathToOpenGLType(const std::string& shaderPath);
}