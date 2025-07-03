#pragma once

#include "glm.hpp"

#include <stdint.h>
#include <memory>


class VertexArray;

enum class EDrawPrimitiveMode
{
	None = 0,
	Points,
	Lines,
	Triangles,
	TriangleStrip,
	TriangleFan
};

struct SDrawSettings
{
	EDrawPrimitiveMode PrimitiveMode = EDrawPrimitiveMode::Triangles;
};

class RenderCommand
{
public:
	static void SetClearColor(const glm::vec4& color);

	static void DrawIndexed(const std::shared_ptr<VertexArray>& vao, uint32_t indexCount = 0, const SDrawSettings& drawSettings = SDrawSettings());

	static void DrawUnindexed(const std::shared_ptr<VertexArray>& vao, const SDrawSettings& drawSettings = SDrawSettings());

	static void ClearColor();

	static void Clear();
};