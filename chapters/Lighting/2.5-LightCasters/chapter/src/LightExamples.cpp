#include "LightExamples.h"

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
#include "Utility/ShaderUtility.h"
#include "Texture/Texture.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

#include "Core/Core.h"
#include "Material/Material.h"
#include "Light/Light.h"


#include "Utility/Log.h"
#include "Utility/UtilityFunctions.h"
#include "Data/Data.h"


extern int g_WindowWidth;
extern int g_WindowHeight;

extern float g_DeltaTime;
extern float g_LastFrameTime;

extern float g_MouseLastX;
extern float g_MouseLastY;

extern bool g_bFirstMouseInput;

extern FlyCamera g_Camera;

int ExecuteDirectionalLightExample()
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

	DirectionalLight directionalLight;

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

	SCube cubeEntity;

	std::shared_ptr<Shader> cubeShader = std::make_shared<Shader>("shaders/directionalLightForCube.vert", "shaders/directionalLightForCube.frag");
	cubeEntity.MaterialData = std::make_shared<Material>(cubeShader);;
	{
		std::shared_ptr<Texture> diffuseTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/wooden_container_2.png", 0);
		cubeEntity.MaterialData->SetDiffuse(diffuseTexture);

		std::shared_ptr<Texture> specularTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/container_2_specular.png", 1);
		cubeEntity.MaterialData->SetSpecular(specularTexture);

		// Fake stub. Change it if you want real Emission, currently just stub is used, so not to break pixel shader
		std::shared_ptr<Texture> emissionTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/BlackTexture.png", 2);
		cubeEntity.MaterialData->SetEmission(emissionTexture);
	}

	cubeEntity.MaterialData->SetShininess(32.0f);

	RenderCommand::SetClearColor({ 0.3f, 0.6f, 0.6f, 1.0f });

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		g_DeltaTime = currentFrame - g_LastFrameTime;
		g_LastFrameTime = currentFrame;


		glfwPollEvents();
		Input::Process(window);

		glm::mat4 view = glm::mat4(1.0f);
		view = g_Camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(g_Camera.GetZoom()), g_WindowWidth / (float)g_WindowHeight, 0.1f, 100.0f);

		cubeShader->Bind();
		cubeShader->SetMat4("view", view);
		cubeShader->SetMat4("projection", projection);

		const glm::mat4 directionalLightTransformMatrix = UtilityFunctions::CalculateTransformMatrix(directionalLight.GetTransform());
		const glm::vec3 lightDirection = UtilityFunctions::GetForwardDirection(directionalLightTransformMatrix);
		cubeShader->SetFloat3("light.direction", glm::mat3(view) * lightDirection);

		cubeShader->SetFloat3("light.ambient", directionalLight.GetAmbient() * (glm::max(0.0f, directionalLight.GetIntensity() / 2.0f)));
		cubeShader->SetFloat3("light.diffuse", directionalLight.GetColor());
		cubeShader->SetFloat("light.intensity", directionalLight.GetIntensity());
		cubeEntity.MaterialData->Bind();

		RenderCommand::ClearColor();

		Renderer::BeginScene(g_Camera);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 cubeModel = glm::mat4(1.0f);
			cubeModel = glm::translate(cubeModel, cubePositions[i]);
			float angle = 20.0f * i;
			cubeModel = glm::rotate(cubeModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			cubeShader->SetMat4("model", cubeModel);
			cubeShader->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(view * cubeModel))));

			Renderer::Draw(cubeVAO, cubeShader);
		}

		Renderer::EndScene();

		ImGuiWrapper::NewFrame();

		ImGui::Begin("Lighting");

		if (ImGui::CollapsingHeader("Directional Light source", ImGuiTreeNodeFlags_None))
		{
			ImGui::PushID("light");

			if (ImGui::TreeNode("Transform"))
			{
				ImGui::SliderFloat("Rotate X", (float*)&directionalLight.GetTransform().Rotation.x, 0.0f, 360.0f);
				ImGui::SliderFloat("Rotate Y", (float*)&directionalLight.GetTransform().Rotation.y, 0.0f, 360.0f);
				ImGui::SliderFloat("Rotate Z", (float*)&directionalLight.GetTransform().Rotation.z, 0.0f, 360.0f);

				ImGui::TreePop();
				ImGui::Spacing();
			}

			if (ImGui::TreeNode("Light properties"))
			{
				ImGui::ColorEdit3("Ambient RGB", glm::value_ptr(*(glm::vec3*)(&directionalLight.GetAmbient())));
				ImGui::ColorEdit3("Diffuse RGB", glm::value_ptr(*(glm::vec3*)(&directionalLight.GetColor())));
				ImGui::SliderFloat("Intensity", (float*)&directionalLight.GetIntensity(), 0.1f, 5.0f);

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

int ExecutePointLightExample()
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

	std::shared_ptr<Shader> cubeUnlitShader = std::make_shared<Shader>("shaders/cubeUnlit.vert", "shaders/cubeUnlit.frag");
	SPointLightCube pointLightCube;
	{
		pointLightCube.VAO = cubeVAO;
		pointLightCube.MaterialData = std::make_shared<SimpleMaterial>(cubeUnlitShader);
		pointLightCube.Transform.Translation.x = 3.0f;

		pointLightCube.SetColor(glm::vec3(1.0f));
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

	SCube cubeEntity;

	std::shared_ptr<Shader> cubeShader = std::make_shared<Shader>("shaders/pointLightForCube.vert", "shaders/pointLightForCube.frag");
	cubeEntity.MaterialData = std::make_shared<Material>(cubeShader);;
	{
		std::shared_ptr<Texture> diffuseTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/wooden_container_2.png", 0);
		cubeEntity.MaterialData->SetDiffuse(diffuseTexture);

		std::shared_ptr<Texture> specularTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/container_2_specular.png", 1);
		cubeEntity.MaterialData->SetSpecular(specularTexture);

		// Fake stub. Change it if you want real Emission, currently just stub is used, so not to break pixel shader
		std::shared_ptr<Texture> emissionTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/BlackTexture.png", 2);
		cubeEntity.MaterialData->SetEmission(emissionTexture);
	}

	cubeEntity.MaterialData->SetShininess(32.0f);

	RenderCommand::SetClearColor({ 0.3f, 0.6f, 0.6f, 1.0f });

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		g_DeltaTime = currentFrame - g_LastFrameTime;
		g_LastFrameTime = currentFrame;


		glfwPollEvents();
		Input::Process(window);

		glm::mat4 view = glm::mat4(1.0f);
		view = g_Camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(g_Camera.GetZoom()), g_WindowWidth / (float)g_WindowHeight, 0.1f, 100.0f);

		cubeShader->Bind();
		cubeShader->SetMat4("view", view);
		cubeShader->SetMat4("projection", projection);

		cubeShader->SetFloat3("light.position", glm::mat4(view) * glm::vec4(pointLightCube.Transform.Translation, 1.0f));

		cubeShader->SetFloat3("light.ambient", glm::vec3(0.2f));
		cubeShader->SetFloat3("light.diffuse", pointLightCube.PointLightData.GetColor());
		cubeShader->SetFloat("light.intensity", pointLightCube.PointLightData.GetIntensity());

		cubeShader->SetFloat("light.constant", pointLightCube.PointLightData.GetConstant());
		cubeShader->SetFloat("light.linear", pointLightCube.PointLightData.GetLinear());
		cubeShader->SetFloat("light.quadratic", pointLightCube.PointLightData.GetQuadratic());

		cubeEntity.MaterialData->Bind();

		RenderCommand::ClearColor();

		Renderer::BeginScene(g_Camera);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 cubeModel = glm::mat4(1.0f);
			cubeModel = glm::translate(cubeModel, cubePositions[i]);
			float angle = 20.0f * i;
			cubeModel = glm::rotate(cubeModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			cubeShader->SetMat4("model", cubeModel);
			cubeShader->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(view * cubeModel))));

			Renderer::Draw(cubeVAO, cubeShader);
		}

		cubeUnlitShader->Bind();
		cubeUnlitShader->SetMat4("view", view);
		cubeUnlitShader->SetMat4("projection", projection);

		glm::mat4 pointLightCubeTransform = UtilityFunctions::CalculateTransformMatrix(pointLightCube.Transform);
		cubeUnlitShader->SetMat4("model", pointLightCubeTransform);
		cubeUnlitShader->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(view * pointLightCubeTransform))));
		pointLightCube.MaterialData->Bind();

		Renderer::Draw(pointLightCube.VAO, cubeUnlitShader);

		Renderer::EndScene();

		ImGuiWrapper::NewFrame();

		ImGui::Begin("Lighting");

		if (ImGui::CollapsingHeader("Point Light source", ImGuiTreeNodeFlags_None))
		{
			ImGui::PushID("light");

			if (ImGui::TreeNode("Transform"))
			{
				ImGui::SliderFloat("Translation X", &pointLightCube.Transform.Translation.x, -30.0f, 30.0f);
				ImGui::SliderFloat("Translation Y", &pointLightCube.Transform.Translation.y, -30.0f, 30.0f);
				ImGui::SliderFloat("Translation Z", &pointLightCube.Transform.Translation.z, -30.0f, 30.0f);

				ImGui::TreePop();
				ImGui::Spacing();
			}

			if (ImGui::TreeNode("Light properties"))
			{
				glm::vec3 color = pointLightCube.PointLightData.GetColor();
				ImGui::ColorEdit3("Color", glm::value_ptr(color));
				pointLightCube.SetColor(color);
				
				float intensity = pointLightCube.PointLightData.GetIntensity();
				ImGui::SliderFloat("Intensity", &intensity, 0.1f, 5.0f);
				pointLightCube.PointLightData.SetIntensity(intensity);

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

int ExecuteSpotlightExample()
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


	SpotLight spotLight;
	spotLight.SetColor(glm::vec3(1.0f));

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

	SCube cubeEntity;

	std::shared_ptr<Shader> cubeShader = std::make_shared<Shader>("shaders/spotLightForCube.vert", "shaders/spotLightForCube.frag");
	cubeEntity.MaterialData = std::make_shared<Material>(cubeShader);;
	{
		std::shared_ptr<Texture> diffuseTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/wooden_container_2.png", 0);
		cubeEntity.MaterialData->SetDiffuse(diffuseTexture);

		std::shared_ptr<Texture> specularTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/container_2_specular.png", 1);
		cubeEntity.MaterialData->SetSpecular(specularTexture);

		// Fake stub. Change it if you want real Emission, currently just stub is used, so not to break pixel shader
		std::shared_ptr<Texture> emissionTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/BlackTexture.png", 2);
		cubeEntity.MaterialData->SetEmission(emissionTexture);
	}

	cubeEntity.MaterialData->SetShininess(32.0f);

	RenderCommand::SetClearColor({ 0.3f, 0.6f, 0.6f, 1.0f });

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		g_DeltaTime = currentFrame - g_LastFrameTime;
		g_LastFrameTime = currentFrame;


		glfwPollEvents();
		Input::Process(window);

		glm::mat4 view = glm::mat4(1.0f);
		view = g_Camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(g_Camera.GetZoom()), g_WindowWidth / (float)g_WindowHeight, 0.1f, 100.0f);

		cubeShader->Bind();
		cubeShader->SetMat4("view", view);
		cubeShader->SetMat4("projection", projection);

		{
			const glm::vec3 spotLightDirection = UtilityFunctions::GetForwardDirection(view);
			spotLight.SetDirection(spotLightDirection);
		}

		cubeShader->SetFloat3("spotLight.position", glm::mat4(view) * glm::vec4(g_Camera.GetPosition(), 1.0f));
		cubeShader->SetFloat3("spotLight.direction", glm::mat3(view) * spotLight.GetDirection());

		cubeShader->SetFloat3("spotLight.diffuse", spotLight.GetColor());
		cubeShader->SetFloat("spotLight.intensity", spotLight.GetIntensity());

		// dot product of inner angle
		cubeShader->SetFloat("spotLight.innerDot", glm::cos(glm::radians(spotLight.GetInnerAngle())));
		cubeShader->SetFloat("spotLight.outerDot", glm::cos(glm::radians(spotLight.GetOuterAngle())));

		cubeEntity.MaterialData->Bind();

		RenderCommand::ClearColor();

		Renderer::BeginScene(g_Camera);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 cubeModel = glm::mat4(1.0f);
			cubeModel = glm::translate(cubeModel, cubePositions[i]);
			float angle = 20.0f * i;
			cubeModel = glm::rotate(cubeModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			cubeShader->SetMat4("model", cubeModel);
			cubeShader->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(view * cubeModel))));

			Renderer::Draw(cubeVAO, cubeShader);
		}

		Renderer::EndScene();

		ImGuiWrapper::NewFrame();

		ImGui::Begin("Lighting");

		if (ImGui::CollapsingHeader("Spot Light source", ImGuiTreeNodeFlags_None))
		{
			ImGui::PushID("light");

			if (ImGui::TreeNode("Light properties"))
			{
				glm::vec3 color = spotLight.GetColor();
				ImGui::ColorEdit3("Color", glm::value_ptr(color));
				spotLight.SetColor(color);

				float intensity = spotLight.GetIntensity();
				ImGui::SliderFloat("Intensity", &intensity, 0.1f, 5.0f);
				spotLight.SetIntensity(intensity);

				float innerHalfConeAngle = spotLight.GetInnerAngle();
				ImGui::SliderFloat("Inner Cone Half-Angle", &innerHalfConeAngle, 0.1f, 120.0f);
				spotLight.SetInnerAngle(innerHalfConeAngle);

				float outerHalfConeAngle = spotLight.GetOuterAngle();
				ImGui::SliderFloat("Outer Cone Half-Angle", &outerHalfConeAngle, 0.1f, 120.0f);
				spotLight.SetOuterAngle(outerHalfConeAngle);

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

