#pragma once


#include <memory>
#include <string>

class VersatileMaterial;
class VertexArray;
class Shader;

class Mesh
{
public:
	Mesh(const std::string& pathToMesh, bool bLoadAsAssimpModel = false);

	std::shared_ptr<VertexArray> GetVAO() const;

	void Bind();

	std::shared_ptr<Shader> GetShader() const;

protected:
	void LoadOneMesh(const std::string& pathToMesh);
	void LoadAsAssimpModel(const std::string& pathToMesh);

	std::shared_ptr<VersatileMaterial> m_Material;

	std::shared_ptr<VertexArray> m_VAO;
};
