#include "Mesh.h"

#include <memory>

#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Material/VersatileMaterial.h"
#include "AssetManager/AssetManager.h"
#include "Texture/Texture.h"
#include "Shader1/Shader.h"
#include "Buffers/IndexBuffer.h"

#include "Utility/Log.h"


Mesh::Mesh(const std::string& pathToMesh, bool bLoadAsAssimpModel /*= false*/)
{
	SSingleMeshLoadHelper outMeshLoadHelper;
	if (AssetManager::LoadMesh(pathToMesh, outMeshLoadHelper) == false)
	{
		LOG_INFO("Failed to load mesh; Path: {0}", pathToMesh);
		return;
	}

	auto& [meshAssetHandle, rawMeshDataPtr] = outMeshLoadHelper.MeshData;
	if (!rawMeshDataPtr)
	{
		LOG_ERROR("Failed to load mesh; Path: {0}", pathToMesh);
		return;
	}

	SRawMeshData& meshData = *rawMeshDataPtr;

	m_VAO = std::make_shared<VertexArray>();
	{
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>((float*)&meshData.Vertices[0], meshData.Vertices.size() * 8); // multiply by 8 because each vertex has 8 floats (3 for position, 3 for normal, 2 for texture coordinates)
		vertexBuffer->SetLayout(BufferLayout
		{
			{ShaderUtility::EShaderDataType::Float3, "aPos"},
			{ShaderUtility::EShaderDataType::Float3, "aNormal"},
			{ShaderUtility::EShaderDataType::Float2, "aTexCoords"}
		});

		m_VAO->AddVertexBuffer(vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(&meshData.Indices[0], (uint32_t)meshData.Indices.size());
		m_VAO->SetIndexBuffer(indexBuffer);
	}


	std::shared_ptr<Shader> meshShader = std::make_shared<Shader>("shaders/MeshLit.vert", "shaders/MeshLit.frag");
	m_Material = std::make_shared<VersatileMaterial>(meshShader);

	m_Material->SetShininess(32.0f);

	int32_t textureUnit = 0;
	for (SAssetHandle textureAssetHandle : meshData.TextureHandleSet)
	{
		const std::string meshTexturePath = AssetManager::GetAssetPath(textureAssetHandle);

		// we only support forward rendering for now and each object is rendered with a single material
		// TextureUnit is bound and overriden for each draw call

		std::shared_ptr<Texture> texture = std::make_shared<Texture>(meshTexturePath, textureUnit);
		++textureUnit;

		const ETextureType textureType = AssetManager::GetCachedTextureType(textureAssetHandle);
		switch (textureType)
		{
			case ETextureType::Diffuse:		m_Material->AddDiffuse(texture);	continue;
			case ETextureType::Specular:	m_Material->AddSpecular(texture);	continue;
			case ETextureType::Emission:	m_Material->AddEmission(texture);	continue;
			default:
			{
				LOG_WARN("Unknown texture type: {0}", static_cast<int>(textureType));
			}
		}
	}

	AssetManager::UpdateUsageCounter(meshAssetHandle);
}

Mesh::Mesh(const std::string& pathToMesh, const std::shared_ptr<Shader>& associatedShader)
{
	SSingleMeshLoadHelper outMeshLoadHelper;
	if (AssetManager::LoadMesh(pathToMesh, outMeshLoadHelper) == false)
	{
		LOG_INFO("Failed to load mesh; Path: {0}", pathToMesh);
		return;
	}

	auto& [meshAssetHandle, rawMeshDataPtr] = outMeshLoadHelper.MeshData;
	if (!rawMeshDataPtr)
	{
		LOG_ERROR("Failed to load mesh; Path: {0}", pathToMesh);
		return;
	}

	SRawMeshData& meshData = *rawMeshDataPtr;

	m_VAO = std::make_shared<VertexArray>();
	{
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>((float*)&meshData.Vertices[0], meshData.Vertices.size() * 8); // multiply by 8 because each vertex has 8 floats (3 for position, 3 for normal, 2 for texture coordinates)
		vertexBuffer->SetLayout(BufferLayout
		{
			{ShaderUtility::EShaderDataType::Float3, "aPos"},
			{ShaderUtility::EShaderDataType::Float3, "aNormal"},
			{ShaderUtility::EShaderDataType::Float2, "aTexCoords"}
		});

		m_VAO->AddVertexBuffer(vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(&meshData.Indices[0], (uint32_t)meshData.Indices.size());
		m_VAO->SetIndexBuffer(indexBuffer);
	}

	m_Material = std::make_shared<VersatileMaterial>(associatedShader);

	m_Material->SetShininess(32.0f);

	int32_t textureUnit = 0;
	for (SAssetHandle textureAssetHandle : meshData.TextureHandleSet)
	{
		const std::string meshTexturePath = AssetManager::GetAssetPath(textureAssetHandle);

		// we only support forward rendering for now and each object is rendered with a single material
		// TextureUnit is bound and overriden for each draw call

		std::shared_ptr<Texture> texture = std::make_shared<Texture>(meshTexturePath, textureUnit);
		++textureUnit;

		const ETextureType textureType = AssetManager::GetCachedTextureType(textureAssetHandle);
		switch (textureType)
		{
		case ETextureType::Diffuse:		m_Material->AddDiffuse(texture);	continue;
		case ETextureType::Specular:	m_Material->AddSpecular(texture);	continue;
		case ETextureType::Emission:	m_Material->AddEmission(texture);	continue;
		default:
		{
			LOG_WARN("Unknown texture type: {0}", static_cast<int>(textureType));
		}
		}
	}

	AssetManager::UpdateUsageCounter(meshAssetHandle);
}

std::shared_ptr<VertexArray> Mesh::GetVAO() const
{
	return m_VAO;
}

void Mesh::Bind()
{
	if (m_VAO.get())
	{
		m_VAO->Bind();
	}

	if (m_Material.get())
	{
		m_Material->Bind();
	}
}

std::shared_ptr<Shader> Mesh::GetShader() const
{
	return m_Material->GetShader();
}

std::shared_ptr<VersatileMaterial> Mesh::GetMaterial() const
{
	return m_Material;
}

const STransform& Mesh::GetTransform() const
{
	return m_Transform;
}

void Mesh::SetTranslation(const glm::vec3& translation)
{
	m_Transform.Translation = translation;
}

void Mesh::SetRotation(const glm::vec3& rotation)
{
	m_Transform.Rotation = rotation;
}

void Mesh::SetScale(const glm::vec3& scale)
{
	m_Transform.Scale = scale;
}

void Mesh::LoadOneMesh(const std::string& pathToMesh)
{
	//auto [meshAssetHandle, rawMeshData] = std::move(AssetManager::LoadMesh(pathToMesh));
	//if (!meshAssetHandle.IsValid())
	//{
	//	LOG_INFO("Failed to load mesh; Path: {0}", pathToMesh);
	//	return;
	//}

	//SRawMeshData& meshData = rawMeshData;

	//m_VAO = std::make_shared<VertexArray>();
	//{
	//	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>((float*)&meshData.Vertices[0], meshData.Vertices.size() * 8); // multiply by 8 because each vertex has 8 floats (3 for position, 3 for normal, 2 for texture coordinates)
	//	vertexBuffer->SetLayout(BufferLayout
	//		{
	//			{ShaderUtility::EShaderDataType::Float3, "aPos"},
	//			{ShaderUtility::EShaderDataType::Float3, "aNormal"},
	//			{ShaderUtility::EShaderDataType::Float2, "aTexCoords"}
	//		});

	//	m_VAO->AddVertexBuffer(vertexBuffer);

	//	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(&meshData.Indices[0], (uint32_t)meshData.Indices.size());
	//	m_VAO->SetIndexBuffer(indexBuffer);
	//}


	//std::shared_ptr<Shader> meshShader = std::make_shared<Shader>("shaders/MeshLit.vert", "shaders/MeshLit.frag");
	//m_Material = std::make_shared<VersatileMaterial>(meshShader);

	//m_Material->SetShininess(32.0f);

	//for (uint32_t i = 0; i < meshData.TextureHandles.size(); ++i)
	//{
	//	const std::string meshTexturePath = AssetManager::GetAssetPath(meshData.TextureHandles[i]);

	//	// we only support forward rendering for now and each object is rendered with a single material
	//	// TextureUnit is bound and overriden for each draw call

	//	bool bHasTransparencyChannel = true;
	//	if (meshTexturePath.find(".png") == std::string::npos)
	//	{
	//		bHasTransparencyChannel = false;
	//	}

	//	std::shared_ptr<Texture> texture = std::make_shared<Texture>(meshTexturePath, i, bHasTransparencyChannel);
	//	const ETextureType textureType = AssetManager::GetCachedTextureType(meshData.TextureHandles[i]);
	//	switch (textureType)
	//	{
	//	case ETextureType::Diffuse:		m_Material->AddDiffuse(texture);	continue;
	//	case ETextureType::Specular:	m_Material->AddSpecular(texture);	continue;
	//	case ETextureType::Emission:	m_Material->AddEmission(texture);	continue;

	//	default:
	//	{
	//		LOG_WARN("Unknown texture type: {0}", static_cast<int>(textureType));
	//		break;
	//	}
	//	}
	//}

	//AssetManager::UpdateUsageCounter(meshAssetHandle);
}

void Mesh::LoadAsAssimpModel(const std::string& pathToMesh)
{
	//auto [assetHandle, rawMeshesData] = std::move(AssetManager::LoadAssimpModel(pathToMesh));
	//if (!assetHandle.IsValid())
	//{
	//	LOG_INFO("Failed to load mesh; Path: {0}", pathToMesh);
	//	return;
	//}

	//SRawMeshData& meshData = rawMeshData;

	//m_VAO = std::make_shared<VertexArray>();
	//{
	//	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>((float*)&meshData.Vertices[0], meshData.Vertices.size() * 8); // multiply by 8 because each vertex has 8 floats (3 for position, 3 for normal, 2 for texture coordinates)
	//	vertexBuffer->SetLayout(BufferLayout
	//	{
	//			{ShaderUtility::EShaderDataType::Float3, "aPos"},
	//			{ShaderUtility::EShaderDataType::Float3, "aNormal"},
	//			{ShaderUtility::EShaderDataType::Float2, "aTexCoords"}
	//	});

	//	m_VAO->AddVertexBuffer(vertexBuffer);

	//	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(&meshData.Indices[0], (uint32_t)meshData.Indices.size());
	//	m_VAO->SetIndexBuffer(indexBuffer);
	//}


	//std::shared_ptr<Shader> meshShader = std::make_shared<Shader>("shaders/MeshLit.vert", "shaders/MeshLit.frag");
	//m_Material = std::make_shared<VersatileMaterial>(meshShader);

	//m_Material->SetShininess(32.0f);

	//for (uint32_t i = 0; i < meshData.TextureHandles.size(); ++i)
	//{
	//	const std::string meshTexturePath = AssetManager::GetAssetPath(meshData.TextureHandles[i]);

	//	// we only support forward rendering for now and each object is rendered with a single material
	//	// TextureUnit is bound and overriden for each draw call
	//	std::shared_ptr<Texture> texture = std::make_shared<Texture>(meshTexturePath, i);
	//	const ETextureType textureType = AssetManager::GetCachedTextureType(meshData.TextureHandles[i]);
	//	switch (textureType)
	//	{
	//	case ETextureType::Diffuse:		m_Material->AddDiffuse(texture);	continue;
	//	case ETextureType::Specular:	m_Material->AddSpecular(texture);	continue;
	//	case ETextureType::Emission:	m_Material->AddEmission(texture);	continue;

	//	default:
	//	{
	//		LOG_WARN("Unknown texture type: {0}", static_cast<int>(textureType));
	//		break;
	//	}
	//	}
	//}
}

