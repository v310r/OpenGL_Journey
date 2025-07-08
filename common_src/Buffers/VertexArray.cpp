#include "VertexArray.h"

#include "glad/glad.h"

#include "Utility/ShaderUtility.h"
#include "Utility/Log.h"


VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_Id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_Id);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_Id);
	if (HasIndexBuffer())
	{
		m_IndexBuffer->Bind();
	}
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

static void SetupAttribPointerForAttribute(uint32_t inIndex, const BufferAttribute& inAttribute, const std::shared_ptr<VertexBuffer>& inVertexBuffer, uint32_t& outIncrementAmount)
{
	using namespace ShaderUtility;

	switch (inAttribute.Type)
	{
		case EShaderDataType::Float:
		case EShaderDataType::Float2:
		case EShaderDataType::Float3:
		case EShaderDataType::Float4:
		case EShaderDataType::Int:
		case EShaderDataType::Int2:
		case EShaderDataType::Int3:
		case EShaderDataType::Int4:
		{
			glEnableVertexAttribArray(inIndex);
			glVertexAttribPointer(inIndex, inAttribute.Count, ConvertShaderDataTypeToOpenGLType(inAttribute.Type), inAttribute.bNormalized ? GL_TRUE : GL_FALSE, inVertexBuffer->GetLayout().GetStride(), (const void*)inAttribute.Offset);
			outIncrementAmount = 1;

			return;
		}
		case EShaderDataType::Mat3:
		{
			outIncrementAmount = 3;
		}
		case EShaderDataType::Mat4:
		{
			if (outIncrementAmount != 3)
			{
				outIncrementAmount = 4;
			}
	
			// either 4 or 3 elements in vector
			const uint32_t vectorCountForMatrix = inAttribute.Count / outIncrementAmount;
			const uint32_t vectorOffset = 4 * vectorCountForMatrix; // 4 bytes per float
	
			uint32_t additionalOffsetPerColumn = 0;
			for (int i = inIndex; i < inIndex + outIncrementAmount; ++i)
			{
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, vectorCountForMatrix, ConvertShaderDataTypeToOpenGLType(inAttribute.Type), inAttribute.bNormalized ? GL_TRUE : GL_FALSE, inVertexBuffer->GetLayout().GetStride(), (const void*)(inAttribute.Offset + (additionalOffsetPerColumn * vectorOffset)));
	
				if (inVertexBuffer->IsIncludingInstanceData())
				{
					glVertexAttribDivisor(i, 1); // This is a per-instance attribute
				}
	
				++additionalOffsetPerColumn;
			}

			return;
		}
		default:
		{
			LOG_ERROR("Unknown shader data type: {0}", static_cast<uint8_t>(inAttribute.Type));
		}
	}
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	glBindVertexArray(m_Id);
	vertexBuffer->Bind();

	if (vertexBuffer->IsIncludingInstanceData())
	{
		SetInstanceSetup(true);
	}

	uint32_t index = m_LastAvailableAttributeIndex, incrementAmount = 1;
	const BufferLayout& layout = vertexBuffer->GetLayout();
	for (const BufferAttribute& attribute : layout)
	{
		incrementAmount = 1;

		SetupAttribPointerForAttribute(index, attribute, vertexBuffer, incrementAmount);

		index += incrementAmount;
	}

	m_VertexBuffers.emplace_back(vertexBuffer);
	m_VertexCount += vertexBuffer->GetVertexCount();

	m_LastAvailableAttributeIndex = index;
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	m_IndexBuffer = indexBuffer;
}

void VertexArray::SetInstanceAmount(uint32_t inAmount /*= 1*/)
{
	m_InstanceAmount = inAmount;
}

uint32_t VertexArray::GetInstanceAmount() const
{
	return m_InstanceAmount;
}

void VertexArray::SetInstanceSetup(bool bValue)
{
	bHasInstanceSetup = bValue;
}

bool VertexArray::HasInstanceSetup() const
{
	return bHasInstanceSetup;
}

uint32_t VertexArray::GetVertexCount() const
{
	return m_VertexCount;
}

bool VertexArray::HasIndexBuffer() const
{
	return m_IndexBuffer.get() != nullptr;
}

uint32_t VertexArray::GetIndexBufferCount() const
{
	return m_IndexBuffer->GetDataCount();
}

