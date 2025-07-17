#include "Renderer.h"

#include "Camera/FlyCamera.h"
#include "Buffers/VertexArray.h"
#include "Shader1/Shader.h"
#include "RenderCommand.h"
#include "Material/Material.h"
#include "Mesh/Mesh.h"
#include "Object/Entity.h"
#include "Utility/UtilityFunctions.h"


Renderer* Renderer::s_RendererInstance = new Renderer();

Renderer* Renderer::Get()
{
	return s_RendererInstance;
}

void Renderer::BeginScene(const FlyCamera& camera, float windowWidth /*= 800.0f*/, float windowHeight /*= 800.0f*/)
{
	Renderer::SetView(camera.GetViewMatrix());
	Renderer::SetProjection(glm::perspective(glm::radians(camera.GetZoom()), windowWidth / windowHeight, 0.1f, 300.0f));
}

void Renderer::BeginSceneOrthographic(const FlyCamera& camera, float nearPlane, float farPlane)
{
	Renderer::SetView(camera.GetViewMatrix());
	Renderer::SetProjection(glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane));
}

void Renderer::Draw(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Shader>& shader, const SDrawSettings& drawSettings /*= SDrawSettings()*/)
{
	shader->Bind();
	// TODO: set camera related uniforms

	if (vao->HasIndexBuffer())
	{
		RenderCommand::DrawIndexed(vao, 0, drawSettings);
	}
	else
	{
		RenderCommand::DrawUnindexed(vao, drawSettings);
	}

}

void Renderer::Draw(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Material>& Material, const SDrawSettings& drawSettings /*= SDrawSettings()*/)
{
	Material->Bind();

	if (vao->HasIndexBuffer())
	{
		RenderCommand::DrawIndexed(vao, 0, drawSettings);
	}
	else
	{
		RenderCommand::DrawUnindexed(vao, drawSettings);
	}
}

void Renderer::Draw(const std::shared_ptr<Mesh>& mesh, const SDrawSettings& drawSettings /*= SDrawSettings()*/)
{
	mesh->Bind();

	if (mesh->GetVAO()->HasIndexBuffer())
	{
		RenderCommand::DrawIndexed(mesh->GetVAO(), 0,  drawSettings);
	}
	else
	{
		RenderCommand::DrawUnindexed(mesh->GetVAO(), drawSettings);
	}
}

void Renderer::Draw(const std::shared_ptr<Entity>& entity, const SDrawSettings& drawSettings /*= SDrawSettings()*/)
{
	for (std::shared_ptr<Mesh> mesh : entity->GetMeshes())
	{
		std::shared_ptr<Shader> shader = mesh->GetShader();
		shader->Bind();

		mesh->Bind();

		glm::mat4 modelMatrix = UtilityFunctions::CalculateTransformMatrix(entity->GetTransform());
		shader->SetMat4("model", modelMatrix);
		//shader->SetMat4("view", Renderer::GetView());
		//shader->SetMat4("projection", Renderer::GetProjection());

		shader->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(Renderer::GetView() * modelMatrix))));

		Renderer::Draw(mesh, drawSettings);
	}
}

void Renderer::EndScene()
{
	// unimplemented
}

void Renderer::SetView(const glm::mat4& viewMatrix)
{
	Renderer::Get()->SetViewImpl(viewMatrix);
}

void Renderer::SetProjection(const glm::mat4& projectionMatrix)
{
	Renderer::Get()->SetProjectionImpl(projectionMatrix);
}

const glm::mat4& Renderer::GetView()
{
	return Renderer::Get()->GetViewImpl();
}

const glm::mat4& Renderer::GetProjection()
{
	return Renderer::Get()->GetProjectionImpl();
}

void Renderer::SetViewImpl(const glm::mat4& viewMatrix)
{
	m_ViewMatrix = viewMatrix;
}
void Renderer::SetProjectionImpl(const glm::mat4& projectionMatrix)
{
	m_ProjectionMatrix = projectionMatrix;
}

const glm::mat4& Renderer::GetViewImpl() const
{
	return m_ViewMatrix;
}

const glm::mat4& Renderer::GetProjectionImpl() const
{
	return m_ProjectionMatrix;
}
