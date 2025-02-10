#include "glad/glad.h"
#include "GLFW/glfw3.h"

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

	
	// notice how I'm playing with colors and buffer changes
	// Also pay attention to the OnWindowSizeChanged function
	glClearColor(0.3f, 0.6f, 0.6f, 1.0f);

	// actually these are unnecessary
	//glClear(GL_COLOR_BUFFER_BIT);
	//glfwSwapBuffers(window);
	//glClear(GL_COLOR_BUFFER_BIT);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ProcessInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}