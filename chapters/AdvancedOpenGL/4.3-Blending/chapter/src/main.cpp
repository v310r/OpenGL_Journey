#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader1/Shader.h"
#include "stb_image.h"

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

#include "Core/Core.h"
#include "Material/Material.h"
#include "Light/Light.h"


#include "Utility/Log.h"
#include "Utility/UtilityFunctions.h"

#include "assimp/Importer.hpp"


int g_WindowWidth = 800;
int g_WindowHeight = 600;

float g_DeltaTime = 0.0f;
float g_LastFrameTime = 0.0f;

float g_MouseLastX = 400.0f;
float g_MouseLastY = 300.0f;

bool g_bFirstMouseInput = true;

FlyCamera g_Camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 g_ClearColor = glm::vec3{ 0.2f, 0.2f, 0.2f };

struct SCube
{
	STransform Transform;

	std::shared_ptr<Material> MaterialData;
};

struct SQuad
{
	STransform Transform;

	std::shared_ptr<Material> MaterialData;
};

void OnImGuiUpdate(SQuad& inPlatformEntity, DirectionalLight& inDirLight);

int main()
{
	Assimp::Importer importer;

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

	LOG_INFO("Importer Count {0}; Importer Index: {1}", importer.GetImporterCount(), importer.GetImporterIndex("*.bah"));

	ImGuiWrapper::Init(window, "#version 460 core");

	GLint maxTextureUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
	LOG_INFO("Max Texture Units: {0}", maxTextureUnits);

	// even though we are using the same vertex data with normals and not using them in the light object shader
	// there is no decrease in performance, because the vertex data is not sent to GPU twice, but only once
	// OpenGL understands which data already reside in GPU and which not "if you are using the same VBO"

	std::shared_ptr<VertexArray> cubeVAO = std::make_shared<VertexArray>();
	{
		std::shared_ptr<VertexBuffer> sharedVertexBuffer = std::make_shared<VertexBuffer>(g_TexturedCubeVerticesWithNormals, g_TexturedCubeVerticesWithNormalsSizeInBytes / sizeof(g_TexturedCubeVerticesWithNormals[0]));
		sharedVertexBuffer->SetLayout(BufferLayout
		{
			{ShaderUtility::EShaderDataType::Float3, "aPos"},
			{ShaderUtility::EShaderDataType::Float3, "aNormal"},
			{ShaderUtility::EShaderDataType::Float2, "aTexCoords"}
		});

		cubeVAO->AddVertexBuffer(sharedVertexBuffer);
	}

	std::shared_ptr<VertexArray> quadVAO = std::make_shared<VertexArray>();
	{
		std::shared_ptr<VertexBuffer> sharedVertexBufferQuad = std::make_shared<VertexBuffer>(g_TexturedQuadVerticesWithNormals, g_TexturedQuadVerticesWithNormalsSizeInBytes / sizeof(g_TexturedQuadVerticesWithNormals[0]));
		sharedVertexBufferQuad->SetLayout(BufferLayout
		{
			{ShaderUtility::EShaderDataType::Float3, "aPos"},
			{ShaderUtility::EShaderDataType::Float3, "aNormal"},
			{ShaderUtility::EShaderDataType::Float2, "aTexCoords"}
		});

		std::shared_ptr<IndexBuffer> sharedIndexBufferQuad = std::make_shared<IndexBuffer>(g_QuadIndices, g_QuadIndicesSizeInBytes / sizeof(g_QuadIndices[0]));

		quadVAO->AddVertexBuffer(sharedVertexBufferQuad);
		quadVAO->SetIndexBuffer(sharedIndexBufferQuad);
	}

	std::shared_ptr<Shader> litShader = std::make_shared<Shader>("shaders/Lit.vert", "shaders/Lit.frag");

	std::shared_ptr<Shader> grassShader = std::make_shared<Shader>("shaders/Lit.vert", "shaders/Grass.frag");

	std::shared_ptr<Shader> windowShader = std::make_shared<Shader>("shaders/Lit.vert", "shaders/Window.frag");

	std::shared_ptr<Texture> stubTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/BlackTexture.png", 0);

	SQuad platformEntity;
	{
		platformEntity.MaterialData = std::make_shared<Material>(litShader);

		std::shared_ptr<Texture> diffuseTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/brickWall.jpg", 1, false);
		platformEntity.MaterialData->SetDiffuse(diffuseTexture);

		platformEntity.MaterialData->SetSpecular(stubTexture);
		platformEntity.MaterialData->SetEmission(stubTexture);

		platformEntity.MaterialData->SetShininess(32.0f);

		platformEntity.Transform.Translation.y = -8.0f;
		platformEntity.Transform.Rotation.x = 270.0f;
		platformEntity.Transform.Scale *= 100.0f;
	}

	// positions are taken from array
	SQuad grassEntityTemplate;
	{
		grassEntityTemplate.MaterialData = std::make_shared<Material>(grassShader);

		std::shared_ptr<Texture> diffuseTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/grass.png", 1, false);
		grassEntityTemplate.MaterialData->SetDiffuse(diffuseTexture);

		grassEntityTemplate.MaterialData->SetShininess(32.0f);
	}

	std::vector<glm::vec3> grassPositions;
	grassPositions.emplace_back(glm::vec3(-1.5f, platformEntity.Transform.Translation.y + 0.55f, -0.48f));
	grassPositions.emplace_back(glm::vec3(1.5f, platformEntity.Transform.Translation.y + 0.55f, 0.51f));
	grassPositions.emplace_back(glm::vec3(0.0f, platformEntity.Transform.Translation.y + 0.55f, 0.6f));
	grassPositions.emplace_back(glm::vec3(-0.3f, platformEntity.Transform.Translation.y + 0.55f, -2.4f));
	grassPositions.emplace_back(glm::vec3(0.5f, platformEntity.Transform.Translation.y + 0.55f, -0.7f));

	SQuad windowEntityTemplate;
	{
		windowEntityTemplate.MaterialData = std::make_shared<Material>(windowShader);

		std::shared_ptr<Texture> diffuseTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/window.png", 1, false);
		windowEntityTemplate.MaterialData->SetDiffuse(diffuseTexture);

		windowEntityTemplate.MaterialData->SetShininess(32.0f);
	}

	std::vector<glm::vec3> windowPositions;
	windowPositions.emplace_back(glm::vec3(-1.5f, platformEntity.Transform.Translation.y + 0.55f, -0.28f));
	windowPositions.emplace_back(glm::vec3(1.5f, platformEntity.Transform.Translation.y + 0.55f, 0.61f));
	windowPositions.emplace_back(glm::vec3(0.0f, platformEntity.Transform.Translation.y + 0.55f, 0.7f));
	windowPositions.emplace_back(glm::vec3(-0.3f, platformEntity.Transform.Translation.y + 0.55f, -2.3f));
	windowPositions.emplace_back(glm::vec3(0.5f, platformEntity.Transform.Translation.y + 0.55f, -0.6f));

	DirectionalLight directionalLight;
	{
		directionalLight.SetRotation(glm::vec3(25.0f, 0.0f, 0.0f));
		directionalLight.SetIntensity(0.5f);
		directionalLight.SetAmbient(glm::vec3(1.0f, 1.0f, 1.0f));
	}

	SCube cubeEntity;
	{
		cubeEntity.MaterialData = std::make_shared<Material>(litShader);

		std::shared_ptr<Texture> diffuseTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/marble.jpg", 2);
		cubeEntity.MaterialData->SetDiffuse(diffuseTexture);

		// Fake stub. Change it if you want real Emission, currently just stub is used, so not to break pixel shader
		//std::shared_ptr<Texture> emissionTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/BlackTexture.png", 4);
		cubeEntity.MaterialData->SetEmission(stubTexture);

		cubeEntity.MaterialData->SetShininess(32.0f);
	}

	glm::vec3 cubePositions[] =
	{
		glm::vec3(0.0f,  platformEntity.Transform.Translation.y + 0.55f,  0.0f),
		glm::vec3(2.0f,  platformEntity.Transform.Translation.y + 0.55f, -3.0f),
		glm::vec3(-1.5f, platformEntity.Transform.Translation.y + 0.55f, -2.5f),
	};

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		g_DeltaTime = currentFrame - g_LastFrameTime;
		g_LastFrameTime = currentFrame;


		glfwPollEvents();
		Input::Process(window);

		Renderer::BeginScene(g_Camera);

		RenderCommand::SetClearColor(glm::vec4(g_ClearColor, 1.0f));
		RenderCommand::ClearColor();

		// PLATFORM
		{
			platformEntity.MaterialData->Bind();

			litShader->SetMat4("view", Renderer::GetView());
			litShader->SetMat4("projection", Renderer::GetProjection());

			litShader->SetDirectionalLight(directionalLight, "directionalLight", directionalLight.GetTransform());

			glm::mat4 platformModel = UtilityFunctions::CalculateTransformMatrix(platformEntity.Transform);
			litShader->SetMat4("model", platformModel);
			litShader->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(Renderer::GetView()* platformModel))));

			Renderer::Draw(quadVAO, litShader);
		}

		{
			cubeEntity.MaterialData->Bind();

			litShader->SetMat4("view", Renderer::GetView());
			litShader->SetMat4("projection", Renderer::GetProjection());

			litShader->SetDirectionalLight(directionalLight, "directionalLight", directionalLight.GetTransform());

			// CUBE ENTITIES
			for (unsigned int i = 0; i < 3; i++)
			{
				glm::mat4 cubeModel = glm::mat4(1.0f);
				cubeModel = glm::translate(cubeModel, cubePositions[i]);
				//float angle = 20.0f * i;
				//cubeModel = glm::rotate(cubeModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				litShader->SetMat4("model", cubeModel);
				litShader->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(Renderer::GetView() * cubeModel))));

				Renderer::Draw(cubeVAO, litShader);
			}
		}

		{
			grassEntityTemplate.MaterialData->Bind();

			grassShader->SetMat4("view", Renderer::GetView());
			grassShader->SetMat4("projection", Renderer::GetProjection());

			// GRASS ENTITIES
			for (unsigned int i = 0; i < grassPositions.size(); i++)
			{
				glm::mat4 grassModel = glm::mat4(1.0f);
				grassModel = glm::translate(grassModel, grassPositions[i]);
				//float angle = 20.0f * i;
				//grassModel = glm::rotate(grassModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				grassShader->SetMat4("model", grassModel);
				grassShader->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(Renderer::GetView()* grassModel))));

				Renderer::Draw(quadVAO, grassShader);
			}

		}

		// windows are rendered last and are sorted by depth
		{
			windowEntityTemplate.MaterialData->Bind();

			windowShader->SetMat4("view", Renderer::GetView());
			windowShader->SetMat4("projection", Renderer::GetProjection());

			// WINDOW ENTITIES
			// sort window positions by depth relative to camera
			std::sort(windowPositions.begin(), windowPositions.end(), [](const glm::vec3& a, const glm::vec3& b)
			{
				return glm::length(g_Camera.GetPosition() - a) > glm::length(g_Camera.GetPosition() - b);
			});

			for (unsigned int i = 0; i < windowPositions.size(); i++)
			{
				glm::mat4 windowModel = glm::mat4(1.0f);
				windowModel = glm::translate(windowModel, windowPositions[i]);
				//float angle = 20.0f * i;
				//grassModel = glm::rotate(grassModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				windowShader->SetMat4("model", windowModel);
				windowShader->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(Renderer::GetView() * windowModel))));

				Renderer::Draw(quadVAO, windowShader);
			}
		}

		Renderer::EndScene();

		OnImGuiUpdate(platformEntity, directionalLight);

		glfwSwapBuffers(window);
	}

	ImGuiWrapper::Cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void OnImGuiUpdate(SQuad& inPlatformEntity, DirectionalLight& inDirLight)
{
	ImGuiWrapper::NewFrame();

	ImGui::Begin("Lighting");

	ImGui::ColorEdit3("Clear Color", glm::value_ptr(g_ClearColor));

	if (ImGui::CollapsingHeader("Platform Entity", ImGuiTreeNodeFlags_None))
	{
		ImGui::PushID("PlatformEntity");

		if (ImGui::TreeNode("Transform"))
		{
			ImGui::DragFloat3("Translation", glm::value_ptr(inPlatformEntity.Transform.Translation), 1.0f, -30.0f, 30.0f);
			ImGui::DragFloat3("Rotation", glm::value_ptr(inPlatformEntity.Transform.Rotation), 1.0f, 0.0f, 360.0f);
			ImGui::DragFloat2("Scale", glm::value_ptr(inPlatformEntity.Transform.Scale), 1.0f, 1.0f, 1000.0f);

			ImGui::TreePop();
			ImGui::Spacing();
		}

		ImGui::PopID();
	}

	if (ImGui::CollapsingHeader("Directional Light source", ImGuiTreeNodeFlags_None))
	{
		ImGui::PushID("DirectionalLight");

		if (ImGui::TreeNode("Properties"))
		{
			if (ImGui::TreeNode("Transform"))
			{
				ImGui::SliderFloat("Rotate X", (float*)&inDirLight.GetTransform().Rotation.x, 0.0f, 360.0f);
				ImGui::SliderFloat("Rotate Y", (float*)&inDirLight.GetTransform().Rotation.y, 0.0f, 360.0f);
				ImGui::SliderFloat("Rotate Z", (float*)&inDirLight.GetTransform().Rotation.z, 0.0f, 360.0f);

				ImGui::TreePop();
				ImGui::Spacing();
			}

			ImGui::ColorEdit3("Ambient RGB", glm::value_ptr(*(glm::vec3*)(&inDirLight.GetAmbient())));
			ImGui::ColorEdit3("Diffuse RGB", glm::value_ptr(*(glm::vec3*)(&inDirLight.GetColor())));
			ImGui::SliderFloat("Intensity", (float*)&inDirLight.GetIntensity(), 0.1f, 5.0f);

			ImGui::TreePop();
			ImGui::Spacing();
		}

		ImGui::PopID();
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGuiWrapper::GetIO()->Framerate, ImGuiWrapper::GetIO()->Framerate);

	ImGui::End();

	ImGuiWrapper::EndFrameAndRender();
}
