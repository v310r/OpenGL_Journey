#pragma once

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glm.hpp"
#include "gtc/type_ptr.hpp"


class PointLight;
class DirectionalLight;
class SpotLight;

struct STransform;

enum class EShaderType : uint8_t
{
	None = 0,
	VertexShader,
	FragmentShader,
	GeometryShader
};

class Shader
{
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath);

	unsigned int GetID() const { return m_ID; }

	void Bind() const { glUseProgram(m_ID); }

	void Unbind() const { glUseProgram(0); }

	void SetBool(std::string_view name, bool value) const;

	void SetInt(std::string_view name, int value) const;

	void SetFloat(std::string_view name, float value) const;
	void SetFloat3(std::string_view name, glm::vec3 value) const;
	void SetFloat4(std::string_view name, glm::vec4 value) const;

	void SetMat4(std::string_view name, const glm::mat4& matrix, int count = 1, bool bTranspose = false) const;

	// better make mat3 as reference, but right now It's too tedious to do for me ))
	// I would need to cache model matrix as mat3 somewhere
	void SetMat3(std::string_view name, const glm::mat3& matrix, int count = 1, bool bTranspose = false) const;


	void SetPointLight(const PointLight& inPointLight, const std::string& inName, const STransform& inTransform);
	void SetDirectionalLight(const DirectionalLight& inDirectionalLight, const std::string& inName, const STransform& inTransform);
	void SetSpotLight(const SpotLight& inSpotLight, const std::string& inName, const STransform& inTransform);

private:
	uint32_t CompileShader(const std::string& shaderPath, EShaderType shaderType) const;

	unsigned int m_ID;
};