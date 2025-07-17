#pragma once

#include <memory>

#include "glm.hpp"

#include "RenderCommand.h"

class FlyCamera;
class VertexArray;
class Shader;
class Material;
class Mesh;
class Entity;

class Renderer
{
public:
	static Renderer* Get();

	static void BeginScene(const FlyCamera& camera, float windowWidth = 800.0f, float windowHeight = 800.0f);

	static void BeginSceneOrthographic(const FlyCamera& camera, float nearPlane = 1.0f, float farPlane = 7.5f);

	static void Draw(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Shader>& shader, const SDrawSettings& drawSettings = SDrawSettings());

	static void Draw(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Material>& Material, const SDrawSettings& drawSettings = SDrawSettings());

	static void Draw(const std::shared_ptr<Mesh>& mesh, const SDrawSettings& drawSettings = SDrawSettings());

	static void Draw(const std::shared_ptr<Entity>& entity, const SDrawSettings& drawSettings = SDrawSettings());


	static void EndScene();

	static void SetView(const glm::mat4& viewMatrix);
	static void SetProjection(const glm::mat4& projectionMatrix);

	static const glm::mat4& GetView();
	static const glm::mat4& GetProjection();

protected:
	void SetViewImpl(const glm::mat4& viewMatrix);
	void SetProjectionImpl(const glm::mat4& projectionMatrix);

	const glm::mat4& GetViewImpl() const;
	const glm::mat4& GetProjectionImpl() const;

	// not usefull right now, as there is only one type of renderer for this project
	static Renderer* s_RendererInstance;

	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
};