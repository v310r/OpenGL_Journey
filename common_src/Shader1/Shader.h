#pragma once

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glm.hpp"
#include "gtc/type_ptr.hpp"

class Shader
{
public:
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	unsigned int GetID() const { return m_ID; }

	void Bind() { glUseProgram(m_ID); }

	void Unbind() { glUseProgram(0); }

	void SetBool(const std::string& name, bool value) const { glUniform1ui(glGetUniformLocation(m_ID, name.data()), static_cast<unsigned int>(value)); }

	void SetInt(const std::string& name, int value) const { glUniform1i(glGetUniformLocation(m_ID, name.data()), value); }

	void SetFloat(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(m_ID, name.data()), value); }

	void SetMat4(const std::string& name, glm::mat4& matrix, int count = 1, bool bTranspose = false) { glUniformMatrix4fv(glGetUniformLocation(m_ID, name.data()), count, bTranspose, glm::value_ptr(matrix)); }


private:
	unsigned int m_ID;
};