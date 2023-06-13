#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader/Shader.h"
#include "iostream"

int g_WindowWidth = 800;
int g_WindowHeight = 600;


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

	GLFWwindow* window = glfwCreateWindow(g_WindowWidth, g_WindowHeight, "LearnOpenGL", nullptr, nullptr);
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

	
	float vertices[] = 
	{
		// vertices				// color
		0.5f, -0.5f, 0.0f,   	1.0f, 0.0f, 0.0f,		// bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		// bottom left
	   	0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f		// top
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// this was done for learning purposes. Notice that VAO should be always Unbound first!
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Shader defaultShader("shaders/shader.vert", "shaders/shader.frag");


	glBindVertexArray(VAO);
	defaultShader.use();
	defaultShader.setFloat("horizontalOffset", 0.5f);

	glClearColor(0.3f, 0.6f, 0.6f, 1.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ProcessInput(window);


		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}