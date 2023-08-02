#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader1/Shader.h"
#include "iostream"
#include "stb_image.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexArray.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/BufferLayout.h"


int g_WindowWidth = 800;
int g_WindowHeight = 600;

float g_DeltaTime = 0.0f;
float g_LastFrameTime = 0.0f;

float g_Yaw = -90.0f;
float g_Pitch = 0.0f;

float g_MouseLastX = 400.0f;
float g_MouseLastY = 300.0f;

bool g_bFirstMouseInput = true;

float g_FOV = 45.0f;

glm::vec3 g_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 g_CameraForwardVector;
glm::vec3 g_UpVector = glm::vec3(0.0f, 1.0f, 0.0f);

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	constexpr float cameraSpeed = 5.0f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		g_CameraPos += g_CameraForwardVector * cameraSpeed * g_DeltaTime;
	}
	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		g_CameraPos -= g_CameraForwardVector * cameraSpeed * g_DeltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		g_CameraPos -= glm::normalize(glm::cross(g_CameraForwardVector, g_UpVector)) * cameraSpeed * g_DeltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		g_CameraPos += glm::normalize(glm::cross(g_CameraForwardVector, g_UpVector)) * cameraSpeed * g_DeltaTime;
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

void OnMouseMoved(GLFWwindow* window, double xPos, double yPos)
{
	if (g_bFirstMouseInput)
	{
		g_MouseLastX = static_cast<float>(xPos);
		g_MouseLastY = static_cast<float>(yPos);
		g_bFirstMouseInput = false;
	}

	float xOffset = static_cast<float>(xPos) - g_MouseLastX;
	float yOffset = static_cast<float>(yPos) - g_MouseLastY;

	g_MouseLastX = static_cast<float>(xPos);
	g_MouseLastY = static_cast<float>(yPos);

	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	g_Yaw += xOffset;
	g_Pitch -= yOffset;

	if (g_Pitch > 89.0f)
	{
		g_Pitch = 89.0f;
	}

	if (g_Pitch < -89.0f)
	{
		g_Pitch = -89.0f;
	}
}

void OnMouseScrolled(GLFWwindow* window, double xOffset, double yOffset)
{
	g_FOV -= static_cast<float>(yOffset);
	if (g_FOV < 1.0f)
	{
		g_FOV = 1.0f;
	}
	else if (g_FOV > 45.0f)
	{
		g_FOV = 45.0f;
	}
}

int main()
{
	glfwInit();


	const char* glsl_version = "#version 460 core";
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glViewport(0, 0, g_WindowWidth, g_WindowHeight);

	glfwSetWindowSizeCallback(window, OnWindowSizeChanged);
	glfwSetCursorPosCallback(window, OnMouseMoved);
	glfwSetScrollCallback(window, OnMouseScrolled);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	
	float vertices[] = 
	{
		// positions         // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // back side
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // front side
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  // left side
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  // right side
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // down side
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // up side
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

	VertexBuffer vb(vertices, sizeof(vertices));

	BufferLayout bl =
	{
		{ShaderUtility::ShaderDataType::Float3, "aPos"},
		{ShaderUtility::ShaderDataType::Float2, "aTexCoord"}
	};

	size_t index = 0;
	for (BufferAttribute& attribute : bl)
	{
		glVertexAttribPointer(index, attribute.Count, ConvertShaderDataTypeToOpenGLType(attribute.Type), attribute.bNormalized ? GL_TRUE : GL_FALSE, bl.GetStride(), (const void*)attribute.Offset);
		glEnableVertexAttribArray(index);
		++index;
	}

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// this was done for learning purposes. Notice that VAO should be always Unbound first!
	glBindVertexArray(0);
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

	glEnable(GL_DEPTH_TEST);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 translationVector = glm::vec3(0.0f, 0.0f, 0.0f);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		g_DeltaTime = currentFrame - g_LastFrameTime;
		g_LastFrameTime = currentFrame;


		glfwPollEvents();
		ProcessInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, translationVector);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			defaultShader.SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glm::vec3 direction;

		direction.x = std::cos(glm::radians(g_Yaw)) * std::cos(glm::radians(g_Pitch));
		direction.y = std::sin(glm::radians(g_Pitch));
		direction.z = std::sin(glm::radians(g_Yaw)) * std::cos(glm::radians(g_Pitch));

		g_CameraForwardVector = glm::normalize(direction);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(g_CameraPos, g_CameraPos + g_CameraForwardVector, g_UpVector);
		defaultShader.SetMat4("view", view);

		glm::mat4 projection = glm::perspective(glm::radians(g_FOV), g_WindowWidth / (float)g_WindowWidth, 0.1f, 100.0f);
		defaultShader.SetMat4("projection", projection);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("square object");
		ImGui::SliderFloat("x offset", &translationVector.x, -1.0f, 1.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();

		ImGui::EndFrame();
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window);
	}


	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}