#pragma once


#include <memory>
#include <string>

#include "Core/Core.h"

class VersatileMaterial;
class VertexArray;
class Shader;

class Mesh
{
public:
	Mesh(const std::string& pathToMesh, bool bLoadAsAssimpModel = false);

	Mesh(const std::string& pathToMesh, const std::shared_ptr<Shader>& associatedShader);

	std::shared_ptr<VertexArray> GetVAO() const;

	void Bind();

	std::shared_ptr<Shader> GetShader() const;

	std::shared_ptr<VersatileMaterial> GetMaterial() const;

	const STransform& GetTransform() const;

	void SetTranslation(const glm::vec3& translation);
	void SetRotation(const glm::vec3& rotation);
	void SetScale(const glm::vec3& scale);

protected:
	void LoadOneMesh(const std::string& pathToMesh);
	void LoadAsAssimpModel(const std::string& pathToMesh);

	std::shared_ptr<VersatileMaterial> m_Material;

	std::shared_ptr<VertexArray> m_VAO;

	STransform m_Transform;
};
