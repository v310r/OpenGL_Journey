#pragma once

#include "glad/glad.h"


namespace ShaderUtility
{
	enum class EShaderDataType
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
}