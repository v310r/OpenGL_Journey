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
	Renderer::SetProjection(glm::perspective(glm::radians(camera.GetZoom()), windowWidth / windowHeight, 0.1f, 100.0f));
}

void Renderer::Draw(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Shader>& shader)
{
	shader->Bind();
	// TODO: set camera related uniforms

	if (vao->HasIndexBuffer())
	{
		RenderCommand::DrawIndexed(vao);
	}
	else
	{
		RenderCommand::DrawUnindexed(vao);
	}

}

void Renderer::Draw(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Material>& Material)
{
	if (vao->HasIndexBuffer())
	{
		RenderCommand::DrawIndexed(vao);
	}
	else
	{
		RenderCommand::DrawUnindexed(vao);
	}
}

void Renderer::Draw(const std::shared_ptr<Mesh>& mesh)
{
	mesh->Bind();

	if (mesh->GetVAO()->HasIndexBuffer())
	{
		RenderCommand::DrawIndexed(mesh->GetVAO());
	}
	else
	{
		RenderCommand::DrawUnindexed(mesh->GetVAO());
	}
}

void Renderer::Draw(const std::shared_ptr<Entity>& entity)
{
	for (std::shared_ptr<Mesh> mesh : entity->GetMeshes())
	{
		std::shared_ptr<Shader> shader = mesh->GetShader();
		shader->Bind();

		mesh->Bind();

		glm::mat4 modelMatrix = UtilityFunctions::CalculateTransformMatrix(entity->GetTransform());
		shader->SetMat4("model", modelMatrix);
		shader->SetMat4("view", Renderer::GetView());
		shader->SetMat4("projection", Renderer::GetProjection());

		shader->SetMat3("normalMatrixTransform", glm::transpose(glm::inverse(glm::mat3(Renderer::GetView() * modelMatrix))));

		Renderer::Draw(mesh);
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
