#include "Entity.h"

#include "Mesh/Mesh.h"


std::vector<std::shared_ptr<Mesh>>& Entity::GetMeshes()
{
	return m_Meshes;
}

void Entity::AddMesh(const std::shared_ptr<Mesh>& mesh)
{
	m_Meshes.push_back(mesh);
}

void Entity::AddMesh(const std::string& meshPath)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(meshPath);
	AddMesh(mesh);
}

void Entity::AddMesh(const std::string& meshPath, const std::shared_ptr<Shader>& associatedShader)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(meshPath, associatedShader);
	AddMesh(mesh);
}

const STransform& Entity::GetTransform() const
{
	return m_Transform;
}

Entity& Entity::SetTransform(const STransform& transform)
{
	m_Transform = transform;
	return *this;
}

Entity& Entity::SetTranslation(const glm::vec3& translation)
{
	m_Transform.Translation = translation;
	return *this;
}

Entity& Entity::SetRotation(const glm::vec3& rotation)
{
	m_Transform.Rotation = rotation;
	return *this;
}

Entity& Entity::SetScale(const glm::vec3& scale)
{
	m_Transform.Scale = scale;
	return *this;
}

const glm::vec3& Entity::GetTranslation() const
{
	return m_Transform.Translation;
}

const glm::vec3& Entity::GetRotation() const
{
	return m_Transform.Rotation;
}

const glm::vec3& Entity::GetScale() const
{
	return m_Transform.Scale;
}
