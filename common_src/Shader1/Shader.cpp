#include "Shader.h"


#include "Light/Light.h"
#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Utility/UtilityFunctions.h"
#include "Utility/Log.h"


Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	const uint32_t vertexShaderId = CompileShader(vertexShaderPath, EShaderType::VertexShader);
	const uint32_t fragmentShaderId = CompileShader(fragmentShaderPath, EShaderType::FragmentShader);

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertexShaderId);
	glAttachShader(m_ID, fragmentShaderId);
	glLinkProgram(m_ID);

	int success;
	char infoLog[512];
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		LOG_ERROR("Shader program linking failed; Info: {0}", infoLog);
	}

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath)
{
	const uint32_t vertexShaderId = CompileShader(vertexShaderPath, EShaderType::VertexShader);
	const uint32_t fragmentShaderId = CompileShader(fragmentShaderPath, EShaderType::FragmentShader);
	const uint32_t geometryShaderId = CompileShader(geometryShaderPath, EShaderType::GeometryShader);

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertexShaderId);
	glAttachShader(m_ID, fragmentShaderId);
	glAttachShader(m_ID, geometryShaderId);

	glLinkProgram(m_ID);


	int success;
	char infoLog[512];
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		LOG_ERROR("Shader program linking failed; Info: {0}", infoLog);
	}

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	glDeleteShader(geometryShaderId);
}

void Shader::SetBool(std::string_view name, bool value) const
{ 
	GLint location = glGetUniformLocation(m_ID, name.data());
	glUniform1ui(location, static_cast<unsigned int>(value));
}

void Shader::SetInt(std::string_view name, int value) const
{ 
	GLint location = glGetUniformLocation(m_ID, name.data());
	glUniform1i(location, value);
}

void Shader::SetFloat(std::string_view name, float value) const
{ 
	GLint location = glGetUniformLocation(m_ID, name.data());
	glUniform1f(location, value);
}

void Shader::SetFloat3(std::string_view name, glm::vec3 value) const
{
	GLint location = glGetUniformLocation(m_ID, name.data());
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetFloat4(std::string_view name, glm::vec4 value) const
{
	GLint location = glGetUniformLocation(m_ID, name.data());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetMat4(std::string_view name, const glm::mat4& matrix, int count, bool bTranspose) const
{
	GLint location = glGetUniformLocation(m_ID, name.data());
	glUniformMatrix4fv(location, count, bTranspose, glm::value_ptr(matrix));
}

void Shader::SetMat3(std::string_view name, const glm::mat3& matrix, int count /*= 1*/, bool bTranspose /*= false*/) const
{
	GLint location = glGetUniformLocation(m_ID, name.data());
	glUniformMatrix3fv(location, count, bTranspose, glm::value_ptr(matrix));
}

void Shader::SetPointLight(const PointLight& inPointLight, const std::string& inName, const STransform& inTransform)
{
	SetFloat3(inName + ".position", Renderer::GetView() * glm::vec4(inTransform.Translation, 1.0f));
	SetFloat3(inName + ".diffuse", inPointLight.GetColor());
	SetFloat(inName + ".intensity", inPointLight.GetIntensity());
	SetFloat(inName + ".constant", inPointLight.GetConstant());
	SetFloat(inName + ".linear", inPointLight.GetLinear());
	SetFloat(inName + ".quadratic", inPointLight.GetQuadratic());
}

void Shader::SetDirectionalLight(const DirectionalLight& inDirectionalLight, const std::string& inName, const STransform& inTransform)
{
	const glm::mat4 directionalLightTransformMatrix = UtilityFunctions::CalculateTransformMatrix(inDirectionalLight.GetTransform());
	const glm::vec3 directionalLightDirection = UtilityFunctions::GetForwardDirection(directionalLightTransformMatrix);
	SetFloat3(inName + ".direction", glm::mat3(Renderer::GetView()) * directionalLightDirection);

	SetFloat3(inName + ".ambient", inDirectionalLight.GetAmbient() * (glm::max(0.0f, inDirectionalLight.GetIntensity())));
	SetFloat3(inName + ".diffuse", inDirectionalLight.GetColor());
	SetFloat(inName + ".intensity", inDirectionalLight.GetIntensity());
}

void Shader::SetSpotLight(const SpotLight& inSpotLight, const std::string& inName, const STransform& inTransform)
{
	SetFloat3("spotlight.position", Renderer::GetView() * glm::vec4(inTransform.Translation, 1.0f));
	SetFloat3("spotlight.direction", glm::mat3(Renderer::GetView()) * inSpotLight.GetDirection());

	SetFloat3("spotlight.diffuse", inSpotLight.GetColor());
	SetFloat("spotlight.intensity", inSpotLight.GetIntensity());

	// dot product of inner angle
	SetFloat("spotlight.innerDot", glm::cos(glm::radians(inSpotLight.GetInnerAngle())));
	SetFloat("spotlight.outerDot", glm::cos(glm::radians(inSpotLight.GetOuterAngle())));
}

uint32_t Shader::CompileShader(const std::string& shaderPath, EShaderType shaderType) const
{
	std::string shaderCode;

	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		shaderFile.open(shaderPath.data());

		std::stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure& ex)
	{
		LOG_ERROR("Shader file not succesfully read: {0}", shaderPath.data());
	}

	const char* const shaderCodeResult = shaderCode.data();

	unsigned int shaderID;
	int success;
	char infoLog[512];

	const uint32_t shaderOpenGLType = ShaderUtility::ConvertShaderTypeToOpenGLType(shaderType);

	shaderID = glCreateShader(shaderOpenGLType);
	glShaderSource(shaderID, 1, &shaderCodeResult, nullptr);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		LOG_ERROR("Shader compilation failed ({0}); Info: {1}", shaderPath.data(), infoLog);
	}

	return shaderID;
}
