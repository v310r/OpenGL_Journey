#pragma once

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	void use() { glUseProgram(m_ID); }

	void setBool(const std::string& name, bool value) const { glUniform1ui(glGetUniformLocation(m_ID, name.data()), static_cast<unsigned int>(value)); }

	void setInt(const std::string& name, int value) const { glUniform1i(glGetUniformLocation(m_ID, name.data()), value); }

	void setFloat(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(m_ID, name.data()), value); }


private:
	unsigned int m_ID;
};