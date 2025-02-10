/*
* Try to display only the center pixels of the texture image on the rectangle in such a way 
* that the individual pixels are getting visible by changing the texture coordinates. 
* Try to set the texture filtering method to GL_NEAREST to see the pixels more clearly
*/


#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader1/Shader.h"
#include "iostream"
#include "stb_image.h"


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

	
	float vertices[] = 
	{
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.70f, 0.70f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.70f, 0.30f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.30f, 0.30f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.30f, 0.70f    // top left 
	};

	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	unsigned int textures[2];
	glGenTextures(2, textures);

	for (size_t i = 0; i < 2; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	stbi_set_flip_vertically_on_load(true);

	int width, height, NumColorChannels;
	std::string woodenContainerPath = (std::string(ASSETS_PATH) + "/wooden_container.jpg");
	unsigned char* data = stbi_load(woodenContainerPath.data(), &width, &height, &NumColorChannels, 0);
	
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, textures[1]);


	std::string awesomeFacePath = (std::string(ASSETS_PATH) + "/awesomeface.png");
	data = stbi_load(awesomeFacePath.data(), &width, &height, &NumColorChannels, 0);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// this was done for learning purposes. Notice that VAO should be always Unbound first!
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Shader defaultShader("shaders/shader.vert", "shaders/shader.frag");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBindVertexArray(VAO);
	defaultShader.Bind();

	defaultShader.SetInt("texture1", 0);
	defaultShader.SetInt("texture2", 1);

	
	glClearColor(0.3f, 0.6f, 0.6f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ProcessInput(window);


		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}