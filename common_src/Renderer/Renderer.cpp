#include "Renderer.h"

#include "Camera/FlyCamera.h"
#include "Buffers/VertexArray.h"
#include "Shader1/Shader.h"
#include "RenderCommand.h"
#include "Material/Material.h"


Renderer* Renderer::s_RendererInstance = new Renderer();

Renderer* Renderer::Get()
{
	return s_RendererInstance;
}

void Renderer::BeginScene(const FlyCamera& camera)
{
	// TODO: cache camera view and projection
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

void Renderer::EndScene()
{
	// unimplemented
}

