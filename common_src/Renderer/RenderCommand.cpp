#include "RenderCommand.h"

#include "glad/glad.h"

#include <memory>

#include "Buffers/VertexArray.h"
#include "Utility/UtilityFunctions.h"


void RenderCommand::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vao, uint32_t indexCount /*= 0*/, const SDrawSettings& drawSettings /*= SDrawSettings()*/)
{
	GLenum mode = UtilityFunctions::ConvertDrawPrimitiveModeToOpenGL(drawSettings.PrimitiveMode);

	vao->Bind();
	uint32_t count = indexCount ? indexCount : vao->GetIndexBufferCount();
	glDrawElements(mode, (GLsizei)count, GL_UNSIGNED_INT, nullptr);
}

void RenderCommand::DrawUnindexed(const std::shared_ptr<VertexArray>& vao, const SDrawSettings& drawSettings /*= SDrawSettings()*/)
{
	GLenum mode = UtilityFunctions::ConvertDrawPrimitiveModeToOpenGL(drawSettings.PrimitiveMode);

	vao->Bind();
	glDrawArrays(mode, 0, (GLsizei)vao->GetVertexCount());
}

void RenderCommand::ClearColor()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderCommand::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

