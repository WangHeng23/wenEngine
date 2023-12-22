#include "platform/opengl/openglBuffer.hpp"

#include <glad/glad.h>

namespace wen {
openglVertexBuffer::openglVertexBuffer(float *vertices, uint32_t size) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

openglVertexBuffer::~openglVertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void openglVertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void openglVertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

openglIndexBuffer::openglIndexBuffer(uint32_t *indices, uint32_t count)
    : m_Count(count) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
                 GL_STATIC_DRAW);
}

openglIndexBuffer::~openglIndexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void openglIndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void openglIndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
}  // namespace wen