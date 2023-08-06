#include "VertexArray.h"

#include "glad/glad.h"


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
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
    glBindVertexArray(m_Id);
    vertexBuffer->Bind();

    uint32_t index = 0;
    const BufferLayout& layout = vertexBuffer->GetLayout();
    for (const BufferAttribute& attribute : layout)
    {
        glVertexAttribPointer(index, attribute.Count, ConvertShaderDataTypeToOpenGLType(attribute.Type), attribute.bNormalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)attribute.Offset);
        glEnableVertexAttribArray(index);
        ++index;
    }

    m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{

}

