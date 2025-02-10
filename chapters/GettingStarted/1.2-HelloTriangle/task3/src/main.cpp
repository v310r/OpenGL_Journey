/*
	Create two shader programs where the second program uses a different fragment shader 
	that outputs the color yellow; draw both triangles again where one outputs the color yellow
*/


#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "iostream"

int g_WindowWidth = 800;
int g_WindowHeight = 600;

const char* g_vertexShaderSource =
"#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\0";

const char* g_fragmentShaderSourceOrange =
"#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* g_fragmentShaderSourceYellow =
"#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";


void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void OnWindowSizeChanged(GLFWwindow* window, int width, int height)
{
	g_WindowWidth = width;
	g_WindowHeight = height;

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
	glClear(GL_COLOR_BUFFER_BIT);
}

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(g_WindowWidth, g_WindowHeight, "OpenGL_Journey", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, g_WindowWidth, g_WindowHeight);

	glfwSetWindowSizeCallback(window, OnWindowSizeChanged);

	
	float vertices1[] = 
	{
		// left triangle
		-0.5f, 0.0f, 0.0f,	
		-0.5f, 0.5f, 0.0f,	
	   	 0.0f, 0.0f, 0.0f
	};

	float vertices2[] =
	{
		 // right triangle
		  0.5f, 0.0f, 0.0f,
		  0.5f, 0.5f, 0.0f,
		  0.0f, 0.0f, 0.0f
	};

	// first triangle
	unsigned int VAO1;
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);

	unsigned int VBO1;
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// first


	// second triangle
	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	unsigned int VBO2;
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// second


	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &g_vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderOrange, 1, &g_fragmentShaderSourceOrange, nullptr);
	glCompileShader(fragmentShaderOrange);

	glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderOrange, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT_Orange::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &g_fragmentShaderSourceYellow, nullptr);
	glCompileShader(fragmentShaderYellow);

	glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderYellow, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT_Yellow::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgramOrange = glCreateProgram();
	glAttachShader(shaderProgramOrange, vertexShader);
	glAttachShader(shaderProgramOrange, fragmentShaderOrange);
	glLinkProgram(shaderProgramOrange);

	glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramOrange, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM_Orange::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(fragmentShaderOrange);

	unsigned int shaderProgramYellow = glCreateProgram();
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);

	glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramYellow, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM_Yellow::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(fragmentShaderYellow);
	glDeleteShader(vertexShader);

	glUseProgram(shaderProgramYellow);


	glClearColor(0.3f, 0.6f, 0.6f, 1.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ProcessInput(window);

		glClear(GL_COLOR_BUFFER_BIT);


		glBindVertexArray(VAO1);
		glUseProgram(shaderProgramOrange);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO2);
		glUseProgram(shaderProgramYellow);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

	}

	glfwTerminate();

	return 0;
}