#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <utility>
#include <vector>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include "Core/Core.h"


struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;

class Texture;

enum aiTextureType;


struct SGUID
{
	uint64_t Value = 0;
};

struct SAssetHandle
{
	SAssetHandle() = default;

	SAssetHandle(uint64_t value)
	{
		GUID.Value = value;
	}

	bool operator==(const SAssetHandle& other) const
	{
		return GUID.Value == other.GUID.Value;
	}

	bool IsValid() const
	{
		return GUID.Value != 0;
	}

	// not really a GUID, but whatever, that should be enough
	SGUID GUID;
};

namespace std
{
	template<>
	struct hash<SAssetHandle>
	{
		size_t operator()(const SAssetHandle& handle) const noexcept
		{
			return handle.GUID.Value;
		}
	};
}

struct SAssetDataBase
{

};

struct SRawTextureData : public SAssetDataBase
{
	uint32_t Width;
	uint32_t Height;

	uint32_t Channels;

	unsigned char* Data = nullptr;

	// OpenGL specific
	unsigned int GPUid;
};

struct SVertex
{
	glm::vec3 Position;

	glm::vec3 Normal;

	glm::vec2 TextureCoords;
};

struct SRawMeshData : public SAssetDataBase
{
public:
	std::vector<SVertex> Vertices;

	std::vector<unsigned int> Indices;

	std::unordered_set<SAssetHandle> TextureHandleSet;
};

// helper struct for loading Assimp model as one mesh
struct SSingleMeshLoadHelper
{
public:
	// index offset for vertices, used only during Assimp node processing
	// not really useful after the mesh is loaded, but we need it to process Assimp nodes
	uint64_t IndexOffset = 0;

	std::pair<SAssetHandle, SRawMeshData*> MeshData;
};

// AssetManager will hold last references to assets, and deallocate assets upon it's own deallocation
// Thus, AssetManager should be deallocated after World/Scene destruction
class AssetManager
{
public:
	static AssetManager& Get();

	static void Destroy();

	static void DeallocateAll();

	static bool TryDeallocate(SAssetHandle handle);

	static bool LoadTexture(const std::string& pathToFile, std::pair<SAssetHandle, SRawTextureData>& outTextureData);

	static bool LoadAssimpModel(const std::string& pathToFile, SSingleMeshLoadHelper& outMeshLoader);

	// just takes index 0 mesh which was loaded from assimp
	static bool LoadMesh(const std::string& pathToFile, SSingleMeshLoadHelper& outMeshLoader);

	static bool IsStillUsed(SAssetHandle handle);

	static uint64_t CountUsage(SAssetHandle handle);

	static std::string GetAssetPath(SAssetHandle handle);

	static ETextureType GetCachedTextureType(SAssetHandle textureHandle);

	static void UpdateUsageCounter(SAssetHandle handle);
	static void DecreaseUsed(SAssetHandle handle);

	void CacheTextureType(SAssetHandle textureHandle, ETextureType type);

protected:
	bool LoadTextureImpl(SAssetHandle handle, std::pair<SAssetHandle, SRawTextureData>& outTextureData);

	bool LoadAssimpModelImpl(SAssetHandle handle, SSingleMeshLoadHelper& OutMeshLoader);

	void ProcessAssimpNode(aiNode* node, const aiScene* scene, SSingleMeshLoadHelper& OutMeshLoader);

	void ProcessAssimpMesh(SAssetHandle parentHandle, aiMesh* mesh, const aiScene* scene, SSingleMeshLoadHelper& OutMeshLoader);

	void LoadAssimpMaterialTextures(SAssetHandle parentHandle, std::vector<SAssetHandle>& outTextures, aiMaterial* material, aiTextureType assimpTextureType);

	uint64_t CountUsageImpl(SAssetHandle handle);

	std::string GetAssetPathImpl(SAssetHandle handle) const;

	ETextureType GetCachedTextureTypeImpl(SAssetHandle textureHandle) const;

	void DeallocateAllImpl();
	bool TryDeallocateImpl(SAssetHandle handle, bool bDeferContainerErase = false);

	// Currently there is no information on who is using the asset
	void UpdateUsageCounterImpl(SAssetHandle handle);

	bool IsStillUsedImpl(SAssetHandle handle) const;

	void DeacreaseUsedImpl(SAssetHandle handle);

	void NullifyCounter(SAssetHandle handle);

	void SaveAssetHandleMappingToString(SAssetHandle handle, const std::string& pathToFile);

	static AssetManager* s_AssetManager;

	// For now all assets that you load, won't be unloaded until application is shutdown
	std::unordered_map<SAssetHandle, SRawTextureData> m_TextureMap;

	//std::unordered_map<std::string, std::shared_ptr<Material>> m_MaterialMap;

	// Assimp may load a whole scene, but for our project we would expect a simple mesh
	// but, nevertheless, we use std::vector in case we might need a whole model
	std::unordered_map<SAssetHandle, SRawMeshData> m_MeshMap;

	mutable std::unordered_map<SAssetHandle, uint64_t> m_AssetUsageCounter;

	mutable std::unordered_map<SAssetHandle, std::string> m_AssetHandleToStringPath;

	mutable std::unordered_map<SAssetHandle, ETextureType> m_TextureHandleToTextureType;
};