#pragma once

#include "glm.hpp"

#include <stdint.h>
#include <memory>


class VertexArray;

class RenderCommand
{
public:
	static void SetClearColor(const glm::vec4& color);

	static void DrawIndexed(const std::shared_ptr<VertexArray>& vao, uint32_t indexCount = 0);

	static void DrawUnindexed(const std::shared_ptr<VertexArray>& vao);

	static void ClearColor();

	static void Clear();
};