#include "AssetManager.h"

#include "glad/glad.h"

#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Texture/Texture.h"

#include "Utility/Log.h"


static ETextureType ConvertAssimpTextureTypeToUserTextureType(aiTextureType type)
{
	switch (type)
	{
		case aiTextureType_DIFFUSE:		return ETextureType::Diffuse;
		case aiTextureType_SPECULAR:	return ETextureType::Specular;
		case aiTextureType_EMISSIVE:	return ETextureType::Emission;
		case aiTextureType_NORMALS:		return ETextureType::Normal;
		case aiTextureType_HEIGHT:		return ETextureType::Height;
		case aiTextureType_SHININESS:	return ETextureType::Shininess;
		case aiTextureType_AMBIENT:		return ETextureType::Ambient;

		default:						break;
	}

	return ETextureType::None;
}

constexpr uint64_t fnv1a_64(const char* str, uint64_t hash = 14695981039346656037ULL)
{
	return (*str == '\0') ? hash : fnv1a_64(str + 1, (hash ^ static_cast<uint64_t>(*str)) * 1099511628211ULL);
}

uint64_t HashString(const std::string& input)
{
	return fnv1a_64(input.c_str());
}

static uint64_t g_AssetAllocationCounter = 0;

AssetManager* AssetManager::Get()
{
	return s_AssetManager;
}

void AssetManager::Destroy()
{
	if (s_AssetManager)
	{
		AssetManager::DeallocateAll();

		delete s_AssetManager;
		s_AssetManager = nullptr;
	}
}

void AssetManager::DeallocateAll()
{
	if (s_AssetManager)
	{
		s_AssetManager->DeallocateAllImpl();
	}
}

bool AssetManager::TryDeallocate(SAssetHandle handle)
{
	return s_AssetManager ? s_AssetManager->TryDeallocateImpl(handle) : false;
}

bool AssetManager::LoadTexture(const std::string& pathToFile, std::pair<SAssetHandle, SRawTextureData>& outTextureData)
{
	// not really a guid, whatever, should be enough for my purposes
	const uint64_t hash = HashString(pathToFile);
	SAssetHandle handle;
	handle.GUID.Value = hash;

	s_AssetManager->SaveAssetHandleMappingToString(handle, pathToFile);

	return s_AssetManager->LoadTextureImpl(handle, outTextureData);
}

bool AssetManager::LoadAssimpModel(const std::string& pathToFile, SSingleMeshLoadHelper& outMeshLoader)
{
	// not really a guid, whatever, should be enough for my purposes
	const uint64_t hash = HashString(pathToFile);
	SAssetHandle handle;
	handle.GUID.Value = hash;

	s_AssetManager->SaveAssetHandleMappingToString(handle, pathToFile);

	return s_AssetManager->LoadAssimpModelImpl(handle, outMeshLoader);
}

bool AssetManager::LoadMesh(const std::string& pathToFile, SSingleMeshLoadHelper& outMeshLoader)
{
	return AssetManager::LoadAssimpModel(pathToFile, outMeshLoader);
	//auto [assetHandle, mesheData] = LoadAssimpModel(pathToFile);
	//if (meshesData.empty())
	//{
	//	return false;
	//}

	//SRawMeshData MeshData;

	//

	//uint32_t VertexIndex = 0, IndicesIndex = 0, TextureHandleIndex = 0;

	//uint64_t indexOffset = 0;
	//for (int i = 0; i < meshesData.size(); ++i)
	//{
	//	const size_t verticesNum = meshesData[i].Vertices.size();
	//	for (int j = 0; j < verticesNum; ++j)
	//	{
	//		SVertex& vertex = meshesData[i].Vertices[j];

	//		MeshData.Vertices.emplace_back(vertex);

	//		//++VertexIndex;
	//	}

	//	const size_t indicesNum = meshesData[i].Indices.size();
	//	for (int j = 0; j < meshesData[i].Indices.size(); ++j)
	//	{

	//		MeshData.Indices.emplace_back(meshesData[i].Indices[j] + indexOffset);

	//		//++IndicesIndex;
	//	}

	//	for (int j = 0; j < meshesData[i].TextureHandles.size(); ++j)
	//	{
	//		//MeshData.TextureHandleSet.insert(meshesData[i].TextureHandles[j]);
	//		// add condition whether it is unique texture
	//		//++TextureHandleIndex
	//	}

	//	indexOffset += indicesNum;
	//}

	//MeshData.TextureHandles = std::move(meshesData[0].TextureHandles); // take only first mesh texture handles

	//return std::make_pair(assetHandle, std::move(MeshData));
}

bool AssetManager::IsStillUsed(SAssetHandle handle)
{
	return s_AssetManager->IsStillUsedImpl(handle);
}

uint64_t AssetManager::CountUsage(SAssetHandle handle)
{
	return s_AssetManager->CountUsageImpl(handle);
}

std::string AssetManager::GetAssetPath(SAssetHandle handle)
{
	return s_AssetManager->GetAssetPathImpl(handle);
}

ETextureType AssetManager::GetCachedTextureType(SAssetHandle textureHandle)
{
	return s_AssetManager->GetCachedTextureTypeImpl(textureHandle);
}

void AssetManager::UpdateUsageCounter(SAssetHandle handle)
{
	s_AssetManager->UpdateUsageCounterImpl(handle);
}

bool AssetManager::LoadTextureImpl(SAssetHandle handle, std::pair<SAssetHandle, SRawTextureData>& outTextureData)
{
	auto iter = m_TextureMap.find(handle);
	if (iter != m_TextureMap.end())
	{
		outTextureData = *iter;
		return true;
	}

	int width = 0, height = 0, numColorChannels = 0;

	SRawTextureData textureRawData;

	const std::string pathToFile = GetAssetPath(handle);

	unsigned char* data = stbi_load(pathToFile.data(), &width, &height, &numColorChannels, 0);
	if (!data)
	{
		outTextureData = std::make_pair(handle, textureRawData);
		return false;
	}

	textureRawData.Data = data;
	textureRawData.Width = width;
	textureRawData.Height = height;
	textureRawData.Channels = numColorChannels;

	unsigned int GpuId;
	glGenTextures(1, &GpuId);

	textureRawData.GPUid = GpuId;

	m_TextureMap.emplace(std::make_pair(handle, textureRawData));
	auto iter2 = m_TextureMap.find(handle);
	if (iter2 != m_TextureMap.end())
	{
		outTextureData = *iter2;
	}

	return true;
}

bool AssetManager::LoadAssimpModelImpl(SAssetHandle handle, SSingleMeshLoadHelper& outMeshLoader)
{
	Assimp::Importer importer;

	const std::string path = GetAssetPath(handle);
	if (path.empty())
	{
		LOG_ERROR("ERROR::ASSIMP::Path is empty");

		return false;
	}

	const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString())

		return false;
	}

	const std::string assetPath = GetAssetPath(handle);

	const std::string directoryStr = assetPath.substr(0, assetPath.find_last_of('/'));

	outMeshLoader.MeshData.first = handle;

	ProcessAssimpNode(scene->mRootNode, scene, outMeshLoader);

	return true;
}

void AssetManager::ProcessAssimpNode(aiNode* node, const aiScene* scene, SSingleMeshLoadHelper& outMeshLoader)
{
	const auto& [meshAssetHandle, rawMeshData] = outMeshLoader.MeshData;

	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		ProcessAssimpMesh(meshAssetHandle, mesh, scene, outMeshLoader);
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessAssimpNode(node->mChildren[i], scene, outMeshLoader);
	}
}

void AssetManager::ProcessAssimpMesh(SAssetHandle parentHandle, aiMesh* mesh, const aiScene* scene, SSingleMeshLoadHelper& outMeshLoader)
{
	auto& [meshAssetHandle, outMeshData] = outMeshLoader.MeshData;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		SVertex vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.TextureCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TextureCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.TextureCoords = glm::vec2(0.0f, 0.0f);
		}

		outMeshData.Vertices.emplace_back(std::move(vertex));
	}

	uint64_t indexOffset = outMeshLoader.IndexOffset;
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			outMeshData.Indices.emplace_back(face.mIndices[j] + outMeshLoader.IndexOffset);
		}

		indexOffset += face.mNumIndices;
	}

	outMeshLoader.IndexOffset = indexOffset;

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<SAssetHandle> outDiffuseMaps;
		LoadAssimpMaterialTextures(parentHandle, outDiffuseMaps, material, aiTextureType_DIFFUSE);
		outMeshData.TextureHandleSet.insert(outDiffuseMaps.begin(), outDiffuseMaps.end());

		std::vector<SAssetHandle> outSpecularMaps;
		LoadAssimpMaterialTextures(parentHandle, outSpecularMaps, material, aiTextureType_SPECULAR);
		outMeshData.TextureHandleSet.insert(outSpecularMaps.begin(), outSpecularMaps.end());

		std::vector<SAssetHandle> outNormalMaps;
		LoadAssimpMaterialTextures(parentHandle, outNormalMaps, material, aiTextureType_NORMALS);
		outMeshData.TextureHandleSet.insert(outNormalMaps.begin(), outNormalMaps.end());

		std::vector<SAssetHandle> outAmbientMaps;
		LoadAssimpMaterialTextures(parentHandle, outAmbientMaps, material, aiTextureType_AMBIENT);
		outMeshData.TextureHandleSet.insert(outAmbientMaps.begin(), outAmbientMaps.end());

		std::vector<SAssetHandle> outHeightMaps;
		LoadAssimpMaterialTextures(parentHandle, outHeightMaps, material, aiTextureType_HEIGHT);
		outMeshData.TextureHandleSet.insert(outHeightMaps.begin(), outHeightMaps.end());

		std::vector<SAssetHandle> outEmissiveMaps;
		LoadAssimpMaterialTextures(parentHandle, outEmissiveMaps, material, aiTextureType_EMISSIVE);
		outMeshData.TextureHandleSet.insert(outEmissiveMaps.begin(), outEmissiveMaps.end());

		std::vector<SAssetHandle> outShininessMaps;
		LoadAssimpMaterialTextures(parentHandle, outShininessMaps, material, aiTextureType_SHININESS);
		outMeshData.TextureHandleSet.insert(outShininessMaps.begin(), outShininessMaps.end());

		std::vector<SAssetHandle> outOpacityMaps;
		LoadAssimpMaterialTextures(parentHandle, outOpacityMaps, material, aiTextureType_OPACITY);
		outMeshData.TextureHandleSet.insert(outOpacityMaps.begin(), outOpacityMaps.end());

		std::vector<SAssetHandle> outDisplacementMaps;
		LoadAssimpMaterialTextures(parentHandle, outDisplacementMaps, material, aiTextureType_DISPLACEMENT);
		outMeshData.TextureHandleSet.insert(outDisplacementMaps.begin(), outDisplacementMaps.end());

		// TODO: mb do these later
		//aiTextureType_LIGHTMAP
		//aiTextureType_REFLECTION

		// TODO: PBR texture types
	}
}

void AssetManager::LoadAssimpMaterialTextures(SAssetHandle parentHandle, std::vector<SAssetHandle>& outTextures, aiMaterial* material, aiTextureType assimpTextureType)
{
	const std::string parentAssetPath = GetAssetPath(parentHandle);

	const std::string textureDirectoryStr = parentAssetPath.substr(0, parentAssetPath.find_last_of('/'));

	const uint32_t textureCount = material->GetTextureCount(assimpTextureType);
	for (unsigned int i = 0; i < textureCount; i++)
	{
		aiString textureFileName;
		material->GetTexture(assimpTextureType, i, &textureFileName);

		const std::string path = textureDirectoryStr + "/" + textureFileName.C_Str();
		SAssetHandle textureHandle;
		textureHandle.GUID.Value = HashString(path);

		auto iter = m_TextureMap.find(textureHandle);
		if (iter == m_TextureMap.end())
		{
			std::pair<SAssetHandle, SRawTextureData> outTextureData;
			if (AssetManager::LoadTexture(path, outTextureData))
			{
				outTextures.emplace_back(outTextureData.first);

				const ETextureType textureType = ConvertAssimpTextureTypeToUserTextureType(assimpTextureType);
				CacheTextureType(outTextureData.first, textureType);
			}
		}
		else
		{
			outTextures.emplace_back(textureHandle);
		}
	}
}

uint64_t AssetManager::CountUsageImpl(SAssetHandle handle)
{
	if (!handle.IsValid())
	{
		return 0;
	}

	auto counterIter = m_AssetUsageCounter.find(handle);
	if (counterIter == m_AssetUsageCounter.end())
	{
		return 0;
	}
	else
	{
		return m_AssetUsageCounter[handle];
	}
}

std::string AssetManager::GetAssetPathImpl(SAssetHandle handle) const
{
	auto iter = m_AssetHandleToStringPath.find(handle);
	if (iter != m_AssetHandleToStringPath.end())
	{
		return m_AssetHandleToStringPath[handle];
	}

	return "";
}

ETextureType AssetManager::GetCachedTextureTypeImpl(SAssetHandle textureHandle) const
{
	auto iter = m_TextureHandleToTextureType.find(textureHandle);
	if (iter != m_TextureHandleToTextureType.end())
	{
		return m_TextureHandleToTextureType[textureHandle];
	}

	return ETextureType::None;
}

void AssetManager::DeallocateAllImpl()
{
	for (auto& [assetHandle, rawTextureData] : m_TextureMap)
	{
		NullifyCounter(assetHandle);
		TryDeallocateImpl(assetHandle, true);
	}

	m_TextureMap.clear();

	for (auto& [assetHandle, rawMeshData] : m_MeshMap)
	{
		NullifyCounter(assetHandle);
		TryDeallocateImpl(assetHandle, true);
	}

	m_MeshMap.clear();
}

bool AssetManager::TryDeallocateImpl(SAssetHandle handle, bool bDeferContainerErase /*= false*/)
{
	auto textureIter = m_TextureMap.find(handle);
	if (textureIter != m_TextureMap.end())
	{
		auto& [assetHandle, rawTextureData] = *textureIter;
		DecreaseUsed(handle);
		if (IsStillUsed(handle))
		{
			return false;
		}

		glDeleteTextures(1, &rawTextureData.GPUid);
		rawTextureData.GPUid = 0;

		if (rawTextureData.Data)
		{
			stbi_image_free(rawTextureData.Data);
		}

		rawTextureData.Data = nullptr;

		m_TextureHandleToTextureType.erase(handle);

		if (!bDeferContainerErase)
		{
			m_TextureMap.erase(textureIter);
		}

		return true;
	}

	auto meshIter = m_MeshMap.find(handle);
	if (meshIter != m_MeshMap.end())
	{
		auto& [assetHandle, rawMeshData] = *meshIter;
		DecreaseUsed(handle);
		if (IsStillUsed(handle))
		{
			return false;
		}

			// TODO: finish

		return true;
	}

	return false;
}

void AssetManager::UpdateUsageCounterImpl(SAssetHandle handle)
{
	auto counterIter = m_AssetUsageCounter.find(handle);
	if (counterIter == m_AssetUsageCounter.end())
	{
		m_AssetUsageCounter.emplace(std::make_pair(handle, 1));
	}
	else
	{
		m_AssetUsageCounter[handle]++;
	}
}

bool AssetManager::IsStillUsedImpl(SAssetHandle handle) const
{
	const auto counterIter = m_AssetUsageCounter.find(handle);
	if (counterIter != m_AssetUsageCounter.end())
	{
		if (m_AssetUsageCounter[handle] > 0)
		{
			return true;
		}
	}

	return false;
}

void AssetManager::DecreaseUsed(SAssetHandle handle)
{
	s_AssetManager->DeacreaseUsedImpl(handle);
}

void AssetManager::DeacreaseUsedImpl(SAssetHandle handle)
{
	auto counterIter = m_AssetUsageCounter.find(handle);
	if (counterIter != m_AssetUsageCounter.end())
	{
		m_AssetUsageCounter[handle]--;
	}
}
void AssetManager::NullifyCounter(SAssetHandle handle)
{
	m_AssetUsageCounter.erase(handle);
}

void AssetManager::SaveAssetHandleMappingToString(SAssetHandle handle, const std::string& pathToFile)
{
	auto iter = m_AssetHandleToStringPath.find(handle);
	if (iter == m_AssetHandleToStringPath.end())
	{
		m_AssetHandleToStringPath.emplace(std::make_pair(handle, pathToFile));
	}
}

void AssetManager::CacheTextureType(SAssetHandle textureHandle, ETextureType type)
{
	auto iter = m_TextureMap.find(textureHandle);
	if (iter != m_TextureMap.end())
	{	
		auto iter = m_TextureHandleToTextureType.find(textureHandle);
		if (iter == m_TextureHandleToTextureType.end())
		{
			m_TextureHandleToTextureType.emplace(std::make_pair(textureHandle, type));
		}
		else
		{
			m_TextureHandleToTextureType[textureHandle] = type;
		}
	}
	else
	{
		LOG_ERROR("Texture handle not found");
	}

}

AssetManager* AssetManager::s_AssetManager = new AssetManager();
