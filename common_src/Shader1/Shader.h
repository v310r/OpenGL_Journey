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

	void Bind() const { glUseProgram(m_ID); }

	void Unbind() const { glUseProgram(0); }

	void SetBool(const std::string& name, bool value) const;

	void SetInt(const std::string& name, int value) const;

	void SetFloat(const std::string& name, float value) const;

	void SetMat4(const std::string& name, glm::mat4& matrix, int count = 1, bool bTranspose = false) const;


private:
	unsigned int m_ID;
};