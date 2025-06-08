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

#include "Material/Material.h"

#include "Utility/Log.h"


int g_WindowWidth = 800;
int g_WindowHeight = 600;

float g_DeltaTime = 0.0f;
float g_LastFrameTime = 0.0f;

float g_MouseLastX = 400.0f;
float g_MouseLastY = 300.0f;

bool g_bFirstMouseInput = true;

FlyCamera g_Camera(glm::vec3(0.0f, 0.0f, 3.0f));

struct SLightProps
{
	glm::vec3 Ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 Specular = glm::vec3(0.1f, 0.1f, 0.1f);
};

struct SLightCube
{
	STransform Transform;

	SLightProps LightProps;
};

struct SCube
{
	STransform Transform;

	std::shared_ptr<Material> MaterialData;
};

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
	LOG_INFO("Max Texture Units: {0}", maxTextureUnits);

	// even though we are using the same vertex data with normals and not using them in the light object shader
	// there is no decrease in performance, because the vertex data is not sent to GPU twice, but only once
	// OpenGL understands which data already reside in GPU and which not "if you are using the same VBO"

	std::shared_ptr<VertexBuffer> sharedVertexBuffer = std::make_shared<VertexBuffer>(g_TexturedCubeVerticesWithNormals, g_TexturedCubeVerticesWithNormalsSizeInBytes / sizeof(g_TexturedCubeVerticesWithNormals[0]));
	sharedVertexBuffer->SetLayout(BufferLayout
	{
		{ShaderUtility::EShaderDataType::Float3, "aPos"},
		{ShaderUtility::EShaderDataType::Float3, "aNormal"},
		{ShaderUtility::EShaderDataType::Float2, "aTexCoords"}
	});

	std::shared_ptr<VertexArray> cubeVAO = std::make_shared<VertexArray>();
	{
		cubeVAO->AddVertexBuffer(sharedVertexBuffer);
	}

	SCube cubeEntity;

	SLightCube lightCubeEntity;
	lightCubeEntity.Transform.Translation = glm::vec3{ 0.0f, 0.0f, -1.5f };

	std::shared_ptr<Shader> cubeShader = std::make_shared<Shader>("shaders/cube.vert", "shaders/cube.frag");
	cubeEntity.MaterialData = std::make_shared<Material>(cubeShader);

	{
		std::shared_ptr<Texture> diffuseTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/wooden_container_2.png", 0);
		cubeEntity.MaterialData->SetDiffuse(diffuseTexture);

		std::shared_ptr<Texture> specularTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/container_2_specular.png", 1);
		cubeEntity.MaterialData->SetSpecular(specularTexture);

		std::shared_ptr<Texture> emissionTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/emission_map_matrix_container.jpg", 2, false);
		cubeEntity.MaterialData->SetEmission(emissionTexture);
	}

	cubeEntity.MaterialData->SetShininess(32.0f);


	std::shared_ptr<VertexArray> lightVAO = std::make_shared<VertexArray>();
	{
		lightVAO->AddVertexBuffer(sharedVertexBuffer);
	}

	std::shared_ptr<Shader> lightingShader = std::make_shared<Shader>("shaders/lighting.vert", "shaders/lighting.frag");

	RenderCommand::SetClearColor({ 0.3f, 0.6f, 0.6f, 1.0f });

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		g_DeltaTime = currentFrame - g_LastFrameTime;
		g_LastFrameTime = currentFrame;


		glfwPollEvents();
		Input::Process(window);

		glm::mat4 cubeModel = glm::mat4(1.0f);
		cubeModel = glm::translate(cubeModel, cubeEntity.Transform.Translation);
		cubeModel = glm::rotate(cubeModel, glm::radians(cubeEntity.Transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		cubeModel = glm::rotate(cubeModel, glm::radians(cubeEntity.Transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		cubeModel = glm::rotate(cubeModel, glm::radians(cubeEntity.Transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		cubeModel = glm::scale(cubeModel, cubeEntity.Transform.Scale);

		glm::mat4 view = glm::mat4(1.0f);
		view = g_Camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(g_Camera.GetZoom()), g_WindowWidth / (float)g_WindowHeight, 0.1f, 100.0f);

		cubeShader->Bind();
		cubeShader->SetMat4("model", cubeModel);
		cubeShader->SetMat4("view", view);
		cubeShader->SetMat4("projection", projection);
		cubeShader->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(view * cubeModel))));

		cubeShader->SetFloat3("light.position", glm::vec3(view * glm::vec4(lightCubeEntity.Transform.Translation, 1.0f)));
		cubeShader->SetFloat3("light.ambient", lightCubeEntity.LightProps.Ambient);
		cubeShader->SetFloat3("light.diffuse", lightCubeEntity.LightProps.Diffuse);
		cubeShader->SetFloat3("light.specular", lightCubeEntity.LightProps.Specular);
		cubeShader->SetFloat("time", glfwGetTime());
		cubeEntity.MaterialData->Bind();

		glm::mat4 lightCubeModel = glm::mat4(1.0f);
		lightCubeModel = glm::translate(lightCubeModel, lightCubeEntity.Transform.Translation);
		lightCubeModel = glm::rotate(lightCubeModel, glm::radians(lightCubeEntity.Transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		lightCubeModel = glm::rotate(lightCubeModel, glm::radians(lightCubeEntity.Transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		lightCubeModel = glm::rotate(lightCubeModel, glm::radians(lightCubeEntity.Transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		lightCubeModel = glm::scale(lightCubeModel, lightCubeEntity.Transform.Scale);

		lightingShader->Bind();
		lightingShader->SetMat4("model", lightCubeModel);
		lightingShader->SetMat4("view", view);
		lightingShader->SetMat4("projection", projection);

		lightingShader->SetFloat3("color", lightCubeEntity.LightProps.Diffuse);

		RenderCommand::ClearColor();

		Renderer::BeginScene(g_Camera);

		Renderer::Draw(cubeVAO, cubeShader);
		Renderer::Draw(lightVAO, lightingShader);

		Renderer::EndScene();

		ImGuiWrapper::NewFrame();

		ImGui::Begin("Lighting");

		if (ImGui::CollapsingHeader("Cube entity", ImGuiTreeNodeFlags_None))
		{
			ImGui::PushID("entity");

			if (ImGui::TreeNode("Transform"))
			{
				ImGui::SliderFloat("Translate X", &cubeEntity.Transform.Translation.x, -10.0f, 10.0f);
				ImGui::SliderFloat("Translate Y", &cubeEntity.Transform.Translation.y, -10.0f, 10.0f);
				ImGui::SliderFloat("Translate Z", &cubeEntity.Transform.Translation.z, -10.0f, 10.0f);

				ImGui::SliderFloat("Rotate X", &cubeEntity.Transform.Rotation.x, 0.0f, 360.0f);
				ImGui::SliderFloat("Rotate Y", &cubeEntity.Transform.Rotation.y, 0.0f, 360.0f);
				ImGui::SliderFloat("Rotate Z", &cubeEntity.Transform.Rotation.z, 0.0f, 360.0f);

				ImGui::SliderFloat("Scale X", &cubeEntity.Transform.Scale.x, 0.1f, 10.0f);
				ImGui::SliderFloat("Scale Y", &cubeEntity.Transform.Scale.y, 0.1f, 10.0f);
				ImGui::SliderFloat("Scale Z", &cubeEntity.Transform.Scale.z, 0.1f, 10.0f);

				ImGui::TreePop();
				ImGui::Spacing();
			}

			if (ImGui::TreeNode("Properties"))
			{
				ImGui::SliderFloat("Shininess value", &cubeEntity.MaterialData->GetShininess(), 0.0f, 1024.0f);

				ImGui::TreePop();
				ImGui::Spacing();
			}

			ImGui::PopID();
		}

		//ImGui::ShowDemoWindow();

		if (ImGui::CollapsingHeader("Light source", ImGuiTreeNodeFlags_None))
		{
			ImGui::PushID("light");

			if (ImGui::TreeNode("Transform"))
			{
				ImGui::SliderFloat("Translate X", &lightCubeEntity.Transform.Translation.x, -10.0f, 10.0f);
				ImGui::SliderFloat("Translate Y", &lightCubeEntity.Transform.Translation.y, -10.0f, 10.0f);
				ImGui::SliderFloat("Translate Z", &lightCubeEntity.Transform.Translation.z, -10.0f, 10.0f);

				ImGui::SliderFloat("Rotate X", &lightCubeEntity.Transform.Rotation.x, 0.0f, 360.0f);
				ImGui::SliderFloat("Rotate Y", &lightCubeEntity.Transform.Rotation.y, 0.0f, 360.0f);
				ImGui::SliderFloat("Rotate Z", &lightCubeEntity.Transform.Rotation.z, 0.0f, 360.0f);

				ImGui::SliderFloat("Scale X", &lightCubeEntity.Transform.Scale.x, 0.1f, 10.0f);
				ImGui::SliderFloat("Scale Y", &lightCubeEntity.Transform.Scale.y, 0.1f, 10.0f);
				ImGui::SliderFloat("Scale Z", &lightCubeEntity.Transform.Scale.z, 0.1f, 10.0f);

				ImGui::TreePop();
				ImGui::Spacing();
			}

			if (ImGui::TreeNode("Light properties"))
			{
				ImGui::ColorEdit3("Ambient RGB", glm::value_ptr(lightCubeEntity.LightProps.Ambient));
				ImGui::ColorEdit3("Diffuse RGB", glm::value_ptr(lightCubeEntity.LightProps.Diffuse));
				ImGui::ColorEdit3("Specular RGB", glm::value_ptr(lightCubeEntity.LightProps.Specular));

				ImGui::TreePop();
				ImGui::Spacing();
			}

			ImGui::PopID();
		}

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