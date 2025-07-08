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

#include "Buffers/Framebuffer.h"
#include "Buffers/UniformBuffer.h"

#include "Texture/CubemapTexture.h"

#include "Utility/Log.h"
#include "Utility/UtilityFunctions.h"

#include "assimp/Importer.hpp"
#include "Mesh/Mesh.h"
#include "Material/VersatileMaterial.h"


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

struct SSimpleCube
{
	STransform Transform;

	std::shared_ptr<Shader> ShaderObject;

	std::shared_ptr<VertexArray> VAO;
};

struct SSkyboxCube
{
	STransform Transform;

	std::shared_ptr<Shader> ShaderObject;

	std::shared_ptr<CubemapTexture> CubemapTextureData;

	std::shared_ptr<VertexArray> VAO;
};

struct SQuad
{
	STransform Transform;

	std::shared_ptr<Material> MaterialData;
};

void OnImGuiUpdate(SQuad& inPlatformEntity, DirectionalLight& inDirLight);

void CreateRockInstances(const std::shared_ptr<Mesh> inInstancedMesh, float inRadius, float inOffset, unsigned inAmount)
{
	unsigned int amount = inAmount;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed	
	float radius = inRadius;
	float offset = inOffset;

	glm::vec3 origin = inInstancedMesh->GetTransform().Translation;

	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(origin.x + x, origin.y + y, origin.z + z));

		// 2. scale: scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}

	std::shared_ptr<VertexBuffer> instanceTransformBuffer = std::make_shared<VertexBuffer>(glm::value_ptr(modelMatrices[0]), amount * 16);
	instanceTransformBuffer->SetLayout(BufferLayout
	{
		{ShaderUtility::EShaderDataType::Mat4, "instanceMatrix"}
	});

	instanceTransformBuffer->SetIsIncludingInstanceData(true);

	inInstancedMesh->GetVAO()->AddVertexBuffer(instanceTransformBuffer);
	inInstancedMesh->GetVAO()->SetInstanceAmount(amount);

	delete[] modelMatrices;
}

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

	std::shared_ptr<VertexArray> skyboxCubeVAO = std::make_shared<VertexArray>();
	{
		std::shared_ptr<VertexBuffer> skyboxVertexBuffer = std::make_shared<VertexBuffer>(g_SkyboxCubeVertices, g_CubeVerticesSizeInBytes / sizeof(g_CubeVertices[0]));
		skyboxVertexBuffer->SetLayout(BufferLayout
		{
			{ShaderUtility::EShaderDataType::Float3, "aPos"}
		});

		skyboxCubeVAO->AddVertexBuffer(skyboxVertexBuffer);
	}

	std::shared_ptr<Shader> litShader = std::make_shared<Shader>("shaders/Lit.vert", "shaders/Lit.frag");

	// TODO: Add universe skybox
	std::shared_ptr<Shader> skyboxShader = std::make_shared<Shader>("shaders/Skybox.vert", "shaders/Skybox.frag");

	std::shared_ptr<Texture> stubTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/BlackTexture.png", 0);

	std::shared_ptr<Shader> meshLitShader = std::make_shared<Shader>("shaders/MeshLit.vert", "shaders/MeshLit.frag");
	std::shared_ptr<Shader> meshUnlitShader = std::make_shared<Shader>("shaders/MeshUnlit.vert", "shaders/MeshUnlit.frag");

	std::shared_ptr<Shader> meshLitInstancedShader = std::make_shared<Shader>("shaders/MeshLitInstanced.vert", "shaders/MeshLitInstanced.frag");

	constexpr uint32_t viewProjBindingPoint = 0;
	std::shared_ptr<UniformBuffer> cameraUniformBuffer = std::make_shared<UniformBuffer>(sizeof(glm::mat4) * 2, viewProjBindingPoint);

	std::shared_ptr<Mesh> planetMesh = std::make_shared<Mesh>(std::string(ASSETS_PATH) + "/planet/planet.obj", meshLitShader);
	{
		planetMesh->SetTranslation(glm::vec3(0.0f, 0.0f, -10.0f));
	}

	std::shared_ptr<Mesh> instancedOuterRockMesh = std::make_shared<Mesh>(std::string(ASSETS_PATH) + "/rock/rock.obj", meshLitInstancedShader);
	{
		instancedOuterRockMesh->SetTranslation(glm::vec3(0.0f, 0.0f, 0.0f));
		CreateRockInstances(instancedOuterRockMesh, 150.0f, 30.0f, 10000);
	}

	std::shared_ptr<Mesh> instancedInnerRockMesh = std::make_shared<Mesh>(std::string(ASSETS_PATH) + "/rock/rock.obj", meshLitInstancedShader);
	{
		instancedInnerRockMesh->SetTranslation(glm::vec3(0.0f, 0.0f, 0.0f));
		CreateRockInstances(instancedInnerRockMesh, 45.0f, 15.0f, 1000);
	}

	std::shared_ptr<Mesh> sunMesh = std::make_shared<Mesh>(std::string(ASSETS_PATH) + "/sun/sol.obj", meshUnlitShader);
	{
		// weird obj file, need to set diffuse texture manually
		std::shared_ptr<Texture> sunDiffuseTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/sun/sun_diffuse.jpg", 1);
		sunMesh->GetMaterial()->AddDiffuse(sunDiffuseTexture);

		AssetManager::Get().CacheTextureType(sunDiffuseTexture->GetTextureHandle(), ETextureType::Diffuse);

		sunMesh->SetTranslation(glm::vec3(-110.0f, 40.0f, -250.0f));
		sunMesh->SetScale(glm::vec3(0.002f, 0.002f, 0.002f));
	}

	SQuad platformEntity;
	{
		platformEntity.MaterialData = std::make_shared<Material>(litShader);

		std::shared_ptr<Texture> diffuseTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/brickWall.jpg", 2, false);
		platformEntity.MaterialData->SetDiffuse(diffuseTexture);

		platformEntity.MaterialData->SetSpecular(stubTexture);
		platformEntity.MaterialData->SetEmission(stubTexture);

		platformEntity.MaterialData->SetShininess(32.0f);

		platformEntity.Transform.Translation.y = -8.0f;
		platformEntity.Transform.Rotation.x = 270.0f;
		platformEntity.Transform.Scale *= 100.0f;
	}

	DirectionalLight directionalLight;
	{
		directionalLight.SetRotation(glm::vec3(25.0f, 0.0f, 0.0f));
		directionalLight.SetIntensity(2.0f);
		directionalLight.SetAmbient(glm::vec3(0.05f, 0.05f, 0.05f));

		directionalLight.SetRotation(glm::vec3(165.0f, 335.0f, 0.0f));
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_PROGRAM_POINT_SIZE);

	std::shared_ptr<CubemapTexture> skyboxTexture = std::make_shared<CubemapTexture>(std::string(ASSETS_PATH) + "/skybox/", CubemapSourceTextureList
	{
		{
			{ "universe_right.png",	CubemapTextureOrientation::Right	},
			{ "universe_left.png",	CubemapTextureOrientation::Left		},
			{ "universe_top.png",	CubemapTextureOrientation::Top		},
			{ "universe_bottom.png", CubemapTextureOrientation::Bottom	},

			// these two below were swapped intentionally to display everything correctly
			{ "universe_back.png",	CubemapTextureOrientation::Front	},
			{ "universe_front.png",	CubemapTextureOrientation::Back		}
		}
	});

	SSkyboxCube skyboxCube;
	{
		skyboxCube.Transform.Translation = glm::vec3(0.0f, 0.0f, 0.0f);
		skyboxCube.Transform.Scale = glm::vec3(1.0f, 1.0f, 1.0f);

		skyboxCube.CubemapTextureData = skyboxTexture;

		skyboxCube.ShaderObject = skyboxShader;

		skyboxCube.VAO = skyboxCubeVAO;
	}


	float instanceAngleRotation = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		g_DeltaTime = currentFrame - g_LastFrameTime;
		g_LastFrameTime = currentFrame;


		glfwPollEvents();
		Input::Process(window);

		// Updates
		planetMesh->SetRotation(glm::vec3(planetMesh->GetTransform().Rotation.x + 1.1f * g_DeltaTime, planetMesh->GetTransform().Rotation.y + 10.1f * g_DeltaTime, planetMesh->GetTransform().Rotation.z + 1.1f * g_DeltaTime));

		sunMesh->SetRotation(glm::vec3(sunMesh->GetTransform().Rotation.x + 0.2f * g_DeltaTime, sunMesh->GetTransform().Rotation.y + 2.03f * g_DeltaTime, sunMesh->GetTransform().Rotation.z + 0.2f * g_DeltaTime));

		Renderer::BeginScene(g_Camera);

		RenderCommand::SetClearColor(glm::vec4(g_ClearColor, 1.0f));
		RenderCommand::Clear();

		cameraUniformBuffer->SetData(&Renderer::GetView(), sizeof(glm::mat4), 0);
		cameraUniformBuffer->SetData(&Renderer::GetProjection(), sizeof(glm::mat4), sizeof(glm::mat4));

		glDepthFunc(GL_LESS);

		// draw planet
		{
			planetMesh->Bind();

			planetMesh->GetShader()->SetDirectionalLight(directionalLight, "directionalLight", directionalLight.GetTransform());

			glm::mat4 planetModel = UtilityFunctions::CalculateTransformMatrix(planetMesh->GetTransform());
			planetMesh->GetShader()->SetMat4("model", planetModel);
			planetMesh->GetShader()->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(Renderer::GetView() * planetModel))));

			Renderer::Draw(planetMesh);
		}

		// draw sun
		{
			sunMesh->Bind();

			glm::mat4 sunModel = UtilityFunctions::CalculateTransformMatrix(sunMesh->GetTransform());
			sunMesh->GetShader()->SetMat4("model", sunModel);
			sunMesh->GetShader()->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(Renderer::GetView() * sunModel))));

			Renderer::Draw(sunMesh);
		}

		// draw outer rock instances
		{
			instancedOuterRockMesh->Bind();

			instancedOuterRockMesh->GetShader()->SetDirectionalLight(directionalLight, "directionalLight", directionalLight.GetTransform());

			Renderer::Draw(instancedOuterRockMesh);
		}

		// draw inner rock instances
		{
			instancedInnerRockMesh->Bind();

			instancedInnerRockMesh->GetShader()->SetDirectionalLight(directionalLight, "directionalLight", directionalLight.GetTransform());

			Renderer::Draw(instancedInnerRockMesh);
		}

		glDepthFunc(GL_LEQUAL);
		skyboxCube.ShaderObject->Bind();

		skyboxCube.ShaderObject->SetMat4("rotationOnlyView", glm::mat4(glm::mat3(Renderer::GetView())));

		skyboxCube.CubemapTextureData->Bind();
		skyboxCube.VAO->Bind();
		Renderer::Draw(skyboxCube.VAO, skyboxCube.ShaderObject);

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
