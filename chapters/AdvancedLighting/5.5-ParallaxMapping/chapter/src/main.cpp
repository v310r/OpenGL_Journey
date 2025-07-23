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
#include "Buffers/Framebuffer2.h"


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

	std::shared_ptr<VertexArray> VAO;
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

void OnImGuiUpdate(SQuad& inPlatformEntity, DirectionalLight& inDirLight, SPointLightCube& inPointLightCube);

static float s_HeightScale = 0.1f;

static unsigned int s_tangentQuadVAO = 0;
static unsigned int s_tangentQuadVBO = 0;
void renderQuadWithTangents()
{
	if (s_tangentQuadVAO == 0)
	{
		// positions
		glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
		glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
		glm::vec3 pos3(1.0f, -1.0f, 0.0f);
		glm::vec3 pos4(1.0f, 1.0f, 0.0f);

		// texture coordinates
		glm::vec2 uv1(0.0f, 1.0f);
		glm::vec2 uv2(0.0f, 0.0f);
		glm::vec2 uv3(1.0f, 0.0f);
		glm::vec2 uv4(1.0f, 1.0f);
		// normal vector
		glm::vec3 nm(0.0f, 0.0f, 1.0f);

		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;
		// triangle 1
		// ----------
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		// triangle 2
		// ----------
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                          // bitangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};

		// configure plane VAO
		glGenVertexArrays(1, &s_tangentQuadVAO);
		glGenBuffers(1, &s_tangentQuadVBO);
		glBindVertexArray(s_tangentQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, s_tangentQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	}

	glBindVertexArray(s_tangentQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

int main()
{
	Assimp::Importer importer;

	glfwInit();

	stbi_set_flip_vertically_on_load(true);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	constexpr int SamplingMSAA4x = 4; // 4x MSAA
	glfwWindowHint(GLFW_SAMPLES, SamplingMSAA4x);

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

	//LOG_INFO("Importer Count {0}; Importer Index: {1}", importer.GetImporterCount(), importer.GetImporterIndex("*.bah"));

	ImGuiWrapper::Init(window, "#version 460 core");

	GLint maxTextureUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
	//LOG_INFO("Max Texture Units: {0}", maxTextureUnits);

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

	std::shared_ptr<Shader> unlitShader = std::make_shared<Shader>("shaders/Unlit.vert", "shaders/Unlit.frag");

	std::shared_ptr<Shader> litDisplacementShader = std::make_shared<Shader>("shaders/LitDisplacement.vert", "shaders/LitDisplacement.frag");

	std::shared_ptr<Shader> skyboxShader = std::make_shared<Shader>("shaders/Skybox.vert", "shaders/Skybox.frag");

	std::shared_ptr<Texture> stubTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/BlackTexture.png", 0);

	constexpr uint32_t viewProjBindingPoint = 0;
	std::shared_ptr<UniformBuffer> cameraUniformBuffer = std::make_shared<UniformBuffer>(sizeof(glm::mat4) * 2, viewProjBindingPoint);

	SQuad platformEntity;
	{
		platformEntity.MaterialData = std::make_shared<Material>(litShader);

		std::shared_ptr<Texture> diffuseTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/wood.png", 1);
		platformEntity.MaterialData->SetDiffuse(diffuseTexture);

		platformEntity.MaterialData->SetSpecular(stubTexture);
		platformEntity.MaterialData->SetEmission(stubTexture);

		platformEntity.MaterialData->SetShininess(0.5f);

		platformEntity.Transform.Translation.y = -8.0f;
		platformEntity.Transform.Rotation.x = 270.0f;
		platformEntity.Transform.Scale *= 100.0f;
	}

	std::shared_ptr<Texture> diffuseBrickTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/parallax/brick_diffuse.jpg", 2);
	std::shared_ptr<Texture> normalBrickTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/parallax/brick_normal.jpg", 3);
	std::shared_ptr<Texture> displacementBrickTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/parallax/brick_displacement.jpg", 4);

	SCube cubeEntity;
	{
		cubeEntity.Transform.Translation = glm::vec3(0.0f, -4.0f, 0.0f);
		cubeEntity.Transform.Scale = glm::vec3(1.0f, 1.0f, 1.0f);

		cubeEntity.VAO = cubeVAO;

		cubeEntity.MaterialData = std::make_shared<Material>(litShader);

		std::shared_ptr<Texture> diffuseTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/wooden_container_2.png", 2);
		cubeEntity.MaterialData->SetDiffuse(diffuseTexture);

		std::shared_ptr<Texture> specularTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/container_2_specular.png", 3);
		cubeEntity.MaterialData->SetSpecular(specularTexture);

		// Fake stub. Change it if you want real Emission, currently just stub is used, so not to break pixel shader
		//std::shared_ptr<Texture> emissionTexture = std::make_shared<Texture>(std::string(ASSETS_PATH) + "/BlackTexture.png", 4);
		cubeEntity.MaterialData->SetEmission(stubTexture);

		cubeEntity.MaterialData->SetShininess(0.5f);
	}

	DirectionalLight directionalLight;
	{
		directionalLight.SetRotation(glm::vec3(45.0f, 45.0f, 0.0f));
		directionalLight.SetIntensity(1.0f);
		directionalLight.SetAmbient(glm::vec3(0.1f, 0.1f, 0.1f));
		directionalLight.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	}

	SPointLightCube pointLightCube;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_PROGRAM_POINT_SIZE);

	// for default framebuffer multisampling works out of the box
	// but for custom framebuffers you'd needs manual manipulation of GL_TEXTURE_2D_MULTISAMPLE, etc.
	glEnable(GL_MULTISAMPLE);

	std::shared_ptr<CubemapTexture> skyboxTexture = std::make_shared<CubemapTexture>(std::string(ASSETS_PATH) + "/skybox/", CubemapSourceTextureList
	{
		{
			{ "universe_right.png",	ECubemapTextureOrientation::Right	},
			{ "universe_left.png",	ECubemapTextureOrientation::Left		},
			{ "universe_top.png",	ECubemapTextureOrientation::Top		},
			{ "universe_bottom.png", ECubemapTextureOrientation::Bottom	},

			// these two below were swapped intentionally to display everything correctly
			{ "universe_back.png",	ECubemapTextureOrientation::Front	},
			{ "universe_front.png",	ECubemapTextureOrientation::Back		}
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

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		g_DeltaTime = currentFrame - g_LastFrameTime;
		g_LastFrameTime = currentFrame;


		glfwPollEvents();
		Input::Process(window);

		Renderer::BeginScene(g_Camera);

		RenderCommand::SetClearColor(glm::vec4(g_ClearColor, 1.0f));
		RenderCommand::Clear();

		cameraUniformBuffer->SetData(&Renderer::GetView(), sizeof(glm::mat4), 0);
		cameraUniformBuffer->SetData(&Renderer::GetProjection(), sizeof(glm::mat4), sizeof(glm::mat4));


		glDepthFunc(GL_LESS);

		// PLATFORM
		{
			platformEntity.MaterialData->Bind();

			litShader->SetDirectionalLight(directionalLight, "directionalLight", directionalLight.GetTransform());

			glm::mat4 platformModel = UtilityFunctions::CalculateTransformMatrix(platformEntity.Transform);
			litShader->SetMat4("model", platformModel);
			litShader->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(Renderer::GetView()* platformModel))));

			Renderer::Draw(quadVAO, litShader);
		}

		{
			litDisplacementShader->Bind();

			glm::mat4 model = glm::mat4(1.0f);
			
			// rotate the quad to show normal mapping from multiple directions
			//model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
			litDisplacementShader->SetMat4("model", model);

			diffuseBrickTexture->BindAndActivateUnit();
			normalBrickTexture->BindAndActivateUnit();
			displacementBrickTexture->BindAndActivateUnit();
			litDisplacementShader->SetInt("diffuseMap", diffuseBrickTexture->GetTextureUnit());
			litDisplacementShader->SetInt("normalMap", normalBrickTexture->GetTextureUnit());
			litDisplacementShader->SetInt("displacementMap", displacementBrickTexture->GetTextureUnit());
			litDisplacementShader->SetFloat("heightScale", s_HeightScale);

			litDisplacementShader->SetDirectionalLight(directionalLight, "directionalLight", directionalLight.GetTransform());

			renderQuadWithTangents();
		}


		glDepthFunc(GL_LEQUAL);
		skyboxCube.ShaderObject->Bind();

		skyboxCube.ShaderObject->SetMat4("rotationOnlyView", glm::mat4(glm::mat3(Renderer::GetView())));

		skyboxCube.CubemapTextureData->Bind();
		skyboxCube.VAO->Bind();
		Renderer::Draw(skyboxCube.VAO, skyboxCube.ShaderObject);

		Renderer::EndScene();

		OnImGuiUpdate(platformEntity, directionalLight, pointLightCube);

		glfwSwapBuffers(window);
	}

	ImGuiWrapper::Cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void OnImGuiUpdate(SQuad& inPlatformEntity, DirectionalLight& inDirLight, SPointLightCube& inPointLightCube)
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

	if (ImGui::CollapsingHeader("heightScale", ImGuiTreeNodeFlags_None))
	{
		ImGui::PushID("height");

		ImGui::SliderFloat("Height Scale", &s_HeightScale, 0.01f, 1.0f);

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

	if (ImGui::CollapsingHeader("Point Light source", ImGuiTreeNodeFlags_None))
	{
		ImGui::PushID("PointLight");

		if (ImGui::TreeNode("Properties"))
		{
			//glm::vec3 color = inPointLightCube.PointLightData.GetColor();
			//ImGui::ColorEdit3("Color", glm::value_ptr(color));
			//inPointLightCube.SetColor(color);

			//float intensity = inPointLightCube.PointLightData.GetIntensity();
			//ImGui::SliderFloat("Intensity", &intensity, 0.1f, 5.0f);
			//inPointLightCube.PointLightData.SetIntensity(intensity);

			//if (ImGui::TreeNode("Transform"))
			//{
			//	ImGui::DragFloat3("Translation", glm::value_ptr(inPointLightCube.Transform.Translation), 0.1f, -30.0f, 30.0f);
			//	ImGui::DragFloat3("Rotation", glm::value_ptr(inPointLightCube.Transform.Rotation), 1.0f, 0.0f, 360.0f);
			//	ImGui::DragFloat3("Scale", glm::value_ptr(inPointLightCube.Transform.Scale), 1.0f, 1.0f, 1000.0f);

			//	ImGui::TreePop();
			//	ImGui::Spacing();
			//}

			ImGui::TreePop();
			ImGui::Spacing();
		}

		ImGui::PopID();
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGuiWrapper::GetIO()->Framerate, ImGuiWrapper::GetIO()->Framerate);

	ImGui::End();

	ImGuiWrapper::EndFrameAndRender();
}
