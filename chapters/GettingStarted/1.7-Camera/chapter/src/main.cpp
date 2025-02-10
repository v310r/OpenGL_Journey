#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader1/Shader.h"
#include "stb_image.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "GUI/ImGuiWrapper.h"
#include "Buffers/Buffers.h"
#include "Utility/Debug.h"
#include "Utility/Callbacks.h"
#include "Input/Input.h"
#include "Camera/FlyCamera.h"
#include "Data/Data.h"
#include "Utility/ShaderUtility.h"
#include "Texture/Texture.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"


int g_WindowWidth = 800;
int g_WindowHeight = 600;

float g_DeltaTime = 0.0f;
float g_LastFrameTime = 0.0f;

float g_MouseLastX = 400.0f;
float g_MouseLastY = 300.0f;

bool g_bFirstMouseInput = true;

FlyCamera g_Camera(glm::vec3(0.0f, 0.0f, 3.0f));


int main()
{
	glfwInit();

	stbi_set_flip_vertically_on_load(true);

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

	//glViewport(0, 0, g_WindowWidth, g_WindowHeight);

	glfwSetWindowSizeCallback(window, Callbacks::OnWindowSizeChanged);
	glfwSetCursorPosCallback(window, Callbacks::OnMouseMoved);
	glfwSetScrollCallback(window, Callbacks::OnMouseScrolled);
	glfwSetMouseButtonCallback(window, Callbacks::OnMouseButtonAction);

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(Debug::OnOpenGLdebugCallback, nullptr);
#endif

	ImGuiWrapper::Init(window, "#version 460 core");

	GLint maxTextureUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
	std::cout << "Max Texture Units: " << maxTextureUnits << std::endl;

	Texture woodenContainerAlbedoTexture(std::string(ASSETS_PATH) + "/wooden_container.jpg", 0, false);

	Texture awesomeFaceAlbedoTexture(std::string(ASSETS_PATH) + "/awesomeface.png", 1);

	std::shared_ptr<VertexArray> VAO = std::make_shared<VertexArray>();
	{
		std::shared_ptr<VertexBuffer> vb = std::make_shared<VertexBuffer>(g_TexturedCubeVertices, g_TexturedCubeVerticesSizeInBytes / sizeof(g_TexturedCubeVertices[0]));
		vb->SetLayout(BufferLayout
		{
			{ShaderUtility::EShaderDataType::Float3, "aPos"},
			{ShaderUtility::EShaderDataType::Float2, "aTexCoord"}
		});

		VAO->AddVertexBuffer(vb);

		//std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(g_CubeIndices, g_CubeIndicesSizeInBytes / sizeof(g_CubeIndices[0]));
		//VAO->SetIndexBuffer(indexBuffer);
	}

	glm::vec3 cubePositions[] =
	{
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

	std::shared_ptr<Shader> defaultShader = std::make_shared<Shader>("shaders/shader.vert", "shaders/shader.frag");

	defaultShader->Bind();

	woodenContainerAlbedoTexture.BindAndActivateUnit();
	defaultShader->SetInt("texture1", woodenContainerAlbedoTexture.GetTextureUnit());

	awesomeFaceAlbedoTexture.BindAndActivateUnit();
	defaultShader->SetInt("texture2", awesomeFaceAlbedoTexture.GetTextureUnit());

	RenderCommand::SetClearColor({ 0.3f, 0.6f, 0.6f, 1.0f });

	glEnable(GL_DEPTH_TEST);

	glm::vec3 translationVector = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotationVector = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scaleVector = glm::vec3(1.0f, 1.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		g_DeltaTime = currentFrame - g_LastFrameTime;
		g_LastFrameTime = currentFrame;


		glfwPollEvents();
		Input::Process(window);

		glm::mat4 view = glm::mat4(1.0f);
		view = g_Camera.GetViewMatrix();
		defaultShader->SetMat4("view", view);

		glm::mat4 projection = glm::perspective(glm::radians(g_Camera.GetZoom()), g_WindowWidth / (float)g_WindowHeight, 0.1f, 100.0f);
		defaultShader->SetMat4("projection", projection);

		RenderCommand::ClearColor();

		Renderer::BeginScene(g_Camera);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, translationVector);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(rotationVector.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotationVector.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotationVector.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, scaleVector);
			defaultShader->SetMat4("model", model);

			Renderer::Draw(VAO, defaultShader);
		}

		Renderer::EndScene();

		ImGuiWrapper::NewFrame();

		ImGui::Begin("Cubes");
		ImGui::SliderFloat("Translate X", &translationVector.x, -1.0f, 1.0f);
		ImGui::SliderFloat("Translate Y", &translationVector.y, -1.0f, 1.0f);
		ImGui::SliderFloat("Translate Z", &translationVector.z, -1.0f, 1.0f);

		ImGui::SliderFloat("Rotate X", &rotationVector.x, 0.0f, 360.0f);
		ImGui::SliderFloat("Rotate Y", &rotationVector.y, 0.0f, 360.0f);
		ImGui::SliderFloat("Rotate Z", &rotationVector.z, 0.0f, 360.0f);

		ImGui::SliderFloat("Scale X", &scaleVector.x, 0.1f, 10.0f);
		ImGui::SliderFloat("Scale Y", &scaleVector.y, 0.1f, 10.0f);
		ImGui::SliderFloat("Scale Z", &scaleVector.z, 0.1f, 10.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGuiWrapper::GetIO()->Framerate, ImGuiWrapper::GetIO()->Framerate);
		ImGui::End();

		ImGuiWrapper::EndFrameAndRender();

		glfwSwapBuffers(window);
	}

	ImGuiWrapper::Cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}