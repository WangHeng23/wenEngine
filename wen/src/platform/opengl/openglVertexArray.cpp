#include "platform/opengl/OpenGLVertexArray.hpp"

#include <glad/glad.h>

namespace wen {
static GLenum shaderDataTypeToOpenGLBaseType(shaderDataType type) {
    switch (type) {
        case shaderDataType::float1:
        case shaderDataType::float2:
        case shaderDataType::float3:
        case shaderDataType::float4:
        case shaderDataType::mat3:
        case shaderDataType::mat4:
            return GL_FLOAT;
        case shaderDataType::int1:
        case shaderDataType::int2:
        case shaderDataType::int3:
        case shaderDataType::int4:
            return GL_INT;
        case shaderDataType::bool1:
            return GL_BOOL;
        default:
            WEN_CORE_ASSERT(false, "Unknown shaderDataType!");
            return 0;
    }
}

openglVertexArray::openglVertexArray() {
    glGenVertexArrays(1, &m_RendererID);
}

openglVertexArray::~openglVertexArray() {
    glDeleteVertexArrays(1, &m_RendererID);
}

void openglVertexArray::bind() const {
    glBindVertexArray(m_RendererID);
}

void openglVertexArray::unbind() const {
    glBindVertexArray(0);
}

void openglVertexArray::addVertexBuffer(const Ref<vertexBuffer> &vertexBuffer) {
    WEN_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(),
                    "Vertex Buffer has no layout!");

    glBindVertexArray(m_RendererID);
    vertexBuffer->bind();

    uint32_t index = 0;
    const auto &layout = vertexBuffer->getLayout();
    for (const auto &element : layout) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, element.getComponentCount(),
                              shaderDataTypeToOpenGLBaseType(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE,
                              layout.getStride(), (const void *)element.offset);
        index++;
    }

    m_VertexBuffers.push_back(vertexBuffer);
}

void openglVertexArray::setIndexBuffer(Ref<indexBuffer> &indexBuffer) {
    glBindVertexArray(m_RendererID);
    indexBuffer->bind();

    m_IndexBuffer = indexBuffer;
}

}  // namespace wen