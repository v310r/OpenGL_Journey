#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Core/Core.h"
#include "glm.hpp"

class Mesh;

// not ECS based
class Entity
{
public:
	std::vector<std::shared_ptr<Mesh>>& GetMeshes();

	void AddMesh(const std::shared_ptr<Mesh>& mesh);

	void AddMesh(const std::string& meshPath);

	const STransform& GetTransform() const;
	Entity& SetTransform(const STransform& transform);

	Entity& SetTranslation(const glm::vec3& translation);
	Entity& SetRotation(const glm::vec3& rotation);
	Entity& SetScale(const glm::vec3& scale);

	const glm::vec3& GetTranslation() const;
	const glm::vec3& GetRotation() const;
	const glm::vec3& GetScale() const;

protected:
	STransform m_Transform;

	std::vector<std::shared_ptr<Mesh>> m_Meshes;
};