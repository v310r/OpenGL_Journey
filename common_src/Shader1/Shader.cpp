#include "Shader.h"


Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);\

	try
	{
		vertexShaderFile.open(vertexShaderPath);
		fragmentShaderFile.open(fragmentShaderPath);

		std::stringstream vertexShaderStream, fragmentShaderStream;

		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexShaderCode = vertexShaderStream.str();
		fragmentShaderCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure& ex)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* const vertexShaderCodeResult = vertexShaderCode.data();
	const char* const fragmentShaderCodeResult = fragmentShaderCode.data();

	unsigned int vertexShaderID, fragmentShaderID;
	int success;
	char infoLog[512];

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderCodeResult, nullptr);
	glCompileShader(vertexShaderID);

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCodeResult, nullptr);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertexShaderID);
	glAttachShader(m_ID, fragmentShaderID);
	glLinkProgram(m_ID);

	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
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

void Shader::SetMat4(std::string_view name, glm::mat4& matrix, int count, bool bTranspose) const
{
	GLint location = glGetUniformLocation(m_ID, name.data());
	glUniformMatrix4fv(location, count, bTranspose, glm::value_ptr(matrix));
}

void Shader::SetMat3(std::string_view name, glm::mat3 matrix, int count /*= 1*/, bool bTranspose /*= false*/) const
{
	GLint location = glGetUniformLocation(m_ID, name.data());
	glUniformMatrix3fv(location, count, bTranspose, glm::value_ptr(matrix));
}
