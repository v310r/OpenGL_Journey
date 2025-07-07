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
#include "Mesh/Mesh.h"
#include "Object/Entity.h"


int g_WindowWidth = 800;
int g_WindowHeight = 600;

float g_DeltaTime = 0.0f;
float g_LastFrameTime = 0.0f;

float g_MouseLastX = 400.0f;
float g_MouseLastY = 300.0f;

bool g_bFirstMouseInput = true;

FlyCamera g_Camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 g_ClearColor = glm::vec3{ 0.3f, 0.6f, 0.6f };

glm::vec3 pointLightPositions[] =
{
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

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

void SetShaderUniformsForDirectionalLight(const std::shared_ptr<Shader>& shader, DirectionalLight& directionalLight)
{
	shader->SetDirectionalLight(directionalLight, "directionalLight", directionalLight.GetTransform());
}

void SetShaderUniformsForPointLights(const std::shared_ptr<Shader>& shader, SPointLightCube& pointLightCube)
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		const std::string pointLightName = std::string("pointLights[") + std::to_string(i) + "]";

		STransform pointLightTransform;
		pointLightTransform.Translation = pointLightPositions[i];
		shader->SetPointLight(pointLightCube.PointLightData, pointLightName, pointLightTransform);
	}
}

void SetShaderUniformsForSpotlight(const std::shared_ptr<Shader>& shader, SpotLight& spotlight)
{
	const glm::vec3 spotLightDirection = UtilityFunctions::GetForwardDirection(Renderer::GetView());
	spotlight.SetDirection(spotLightDirection);
	spotlight.SetTranslation(g_Camera.GetPosition());

	shader->SetSpotLight(spotlight, "spotLight", spotlight.GetTransform());
}

void OnImGuiUpdate(SQuad& inPlatformEntity, DirectionalLight& inDirLight, SPointLightCube& inPointLightCube, SpotLight& inSpotLight);

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

	{
		Assimp::Importer importer;
		LOG_INFO("Importer Count {0}; Importer Index: {1}", importer.GetImporterCount(), importer.GetImporterIndex("*.bah"));
	}

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

	std::shared_ptr<Shader> unlitShader = std::make_shared<Shader>("shaders/QuadUnlit.vert", "shaders/QuadUnlit.frag");
	std::shared_ptr<Shader> litShader = std::make_shared<Shader>("shaders/QuadLit.vert", "shaders/QuadLit.frag");

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

	std::shared_ptr<Entity> backpackEntity = std::make_shared<Entity>();
	{
		STransform backpackTransform;// = platformEntity.Transform;
		backpackTransform.Translation = glm::vec3(0.0f, 0.0f, 0.0f);
		backpackEntity->SetTransform(backpackTransform);

		backpackEntity->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

		backpackEntity->AddMesh(std::string(ASSETS_PATH) + "/backpack/backpack.obj");
	}

	//std::shared_ptr<Entity> cubeEntity = std::make_shared<Entity>();
	//{
	//	STransform backpackTransform; //platformEntity.Transform;
	//	backpackTransform.Translation = glm::vec3(0.0f, 0.0f, 0.0f);
	//	cubeEntity->SetTransform(backpackTransform);

	//	cubeEntity->SetScale(glm::vec3(1.0f, 1.0f, 1.0f)).SetRotation(glm::vec3(0.0f, 180.0f, 0.0f));

	//	cubeEntity->AddMesh(std::string(ASSETS_PATH) + "/cube/cube.obj");
	//}

	SpotLight spotLight;

	SPointLightCube pointLightCube;
	{
		pointLightCube.VAO = cubeVAO;
		pointLightCube.MaterialData = std::make_shared<SimpleMaterial>(unlitShader);
		pointLightCube.SetColor(glm::vec3(1.0f));
	}

	DirectionalLight directionalLight;
	{
		directionalLight.SetRotation(glm::vec3(25.0f, 0.0f, 0.0f));
	}

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		g_DeltaTime = currentFrame - g_LastFrameTime;
		g_LastFrameTime = currentFrame;


		glfwPollEvents();
		Input::Process(window);

		Renderer::BeginScene(g_Camera, g_WindowWidth, g_WindowHeight);

		litShader->Bind();
		litShader->SetMat4("view", Renderer::GetView());
		litShader->SetMat4("projection", Renderer::GetProjection());

		SetShaderUniformsForDirectionalLight(litShader, directionalLight);
		SetShaderUniformsForPointLights(litShader, pointLightCube);
		SetShaderUniformsForSpotlight(litShader, spotLight);

		RenderCommand::SetClearColor(glm::vec4(g_ClearColor, 1.0f));
		RenderCommand::ClearColor();

		// PLATFORM
		glm::mat4 platformModel = UtilityFunctions::CalculateTransformMatrix(platformEntity.Transform);
		litShader->SetMat4("model", platformModel);
		litShader->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(Renderer::GetView() * platformModel))));

		platformEntity.MaterialData->Bind();

		Renderer::Draw(quadVAO, litShader);

		for (const std::shared_ptr<Mesh>& backpackMesh : backpackEntity->GetMeshes())
		{
			backpackMesh->GetShader()->Bind();

			SetShaderUniformsForDirectionalLight(backpackMesh->GetShader(), directionalLight);
			SetShaderUniformsForPointLights(backpackMesh->GetShader(), pointLightCube);
			SetShaderUniformsForSpotlight(backpackMesh->GetShader(), spotLight);

			backpackMesh->GetShader()->SetMat4("view", Renderer::GetView());
			backpackMesh->GetShader()->SetMat4("projection", Renderer::GetProjection());
		}

		Renderer::Draw(backpackEntity);
		//Renderer::Draw(cubeEntity);

		unlitShader->Bind();
		unlitShader->SetMat4("view", Renderer::GetView());
		unlitShader->SetMat4("projection", Renderer::GetProjection());

		pointLightCube.MaterialData->Bind();

		// CUBE POINT LIGHT ENTITIES
		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 cubePointLightModel = glm::mat4(1.0f);
			cubePointLightModel = glm::translate(cubePointLightModel, pointLightPositions[i]);
			unlitShader->SetMat4("model", cubePointLightModel);
			unlitShader->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(Renderer::GetView() * cubePointLightModel))));

			Renderer::Draw(cubeVAO, unlitShader);
		}

		Renderer::EndScene();

		OnImGuiUpdate(platformEntity, directionalLight, pointLightCube, spotLight);

		glfwSwapBuffers(window);
	}

	ImGuiWrapper::Cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void OnImGuiUpdate(SQuad& inPlatformEntity, DirectionalLight& inDirLight, SPointLightCube& inPointLightCube, SpotLight& inSpotLight)
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

	if (ImGui::CollapsingHeader("Point Light sources", ImGuiTreeNodeFlags_None))
	{
		ImGui::PushID("PointLight");

		if (ImGui::TreeNode("Properties"))
		{
			glm::vec3 color = inPointLightCube.PointLightData.GetColor();
			ImGui::ColorEdit3("Color", glm::value_ptr(color));
			inPointLightCube.SetColor(color);

			float intensity = inPointLightCube.PointLightData.GetIntensity();
			ImGui::SliderFloat("Intensity", &intensity, 0.1f, 5.0f);
			inPointLightCube.PointLightData.SetIntensity(intensity);

			ImGui::TreePop();
			ImGui::Spacing();
		}

		ImGui::PopID();
	}

	if (ImGui::CollapsingHeader("Spot Light source", ImGuiTreeNodeFlags_None))
	{
		ImGui::PushID("Spotlight");

		if (ImGui::TreeNode("Properties"))
		{
			glm::vec3 color = inSpotLight.GetColor();
			ImGui::ColorEdit3("Color", glm::value_ptr(color));
			inSpotLight.SetColor(color);

			float intensity = inSpotLight.GetIntensity();
			ImGui::SliderFloat("Intensity", &intensity, 0.1f, 5.0f);
			inSpotLight.SetIntensity(intensity);

			float innerHalfConeAngle = inSpotLight.GetInnerAngle();
			ImGui::SliderFloat("Inner Cone Half-Angle", &innerHalfConeAngle, 0.1f, 120.0f);
			inSpotLight.SetInnerAngle(innerHalfConeAngle);

			float outerHalfConeAngle = inSpotLight.GetOuterAngle();
			ImGui::SliderFloat("Outer Cone Half-Angle", &outerHalfConeAngle, 0.1f, 120.0f);
			inSpotLight.SetOuterAngle(outerHalfConeAngle);

			ImGui::TreePop();
			ImGui::Spacing();
		}

		ImGui::PopID();
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGuiWrapper::GetIO()->Framerate, ImGuiWrapper::GetIO()->Framerate);

	ImGui::End();

	ImGuiWrapper::EndFrameAndRender();
}
