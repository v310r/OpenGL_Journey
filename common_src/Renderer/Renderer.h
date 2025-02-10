#pragma once

#include <memory>

class FlyCamera;
class VertexArray;
class Shader;
class Material;

class Renderer
{
public:
	static Renderer* Get();

	static void BeginScene(const FlyCamera& camera);

	static void Draw(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Shader>& shader);

	static void Draw(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Material>& Material);

	static void EndScene();

protected:
	// not usefull right now, as there is only one type of renderer for this project
	static Renderer* s_RendererInstance;
};