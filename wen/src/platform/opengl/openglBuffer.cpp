#include "platform/opengl/openglBuffer.hpp"
#include "wen/debug/instrumentor.hpp"

#include <glad/glad.h>

namespace wen {
openglVertexBuffer::openglVertexBuffer(float *vertices, uint32_t size) {
    WEN_PROFILE_FUNCTION();
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

openglVertexBuffer::~openglVertexBuffer() {
    WEN_PROFILE_FUNCTION();
    glDeleteBuffers(1, &m_RendererID);
}

void openglVertexBuffer::bind() const {
    WEN_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void openglVertexBuffer::unbind() const {
    WEN_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

openglIndexBuffer::openglIndexBuffer(uint32_t *indices, uint32_t count)
    : m_Count(count) {
    WEN_PROFILE_FUNCTION();
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
                 GL_STATIC_DRAW);
}

openglIndexBuffer::~openglIndexBuffer() {
    WEN_PROFILE_FUNCTION();
    glDeleteBuffers(1, &m_RendererID);
}

void openglIndexBuffer::bind() const {
    WEN_PROFILE_FUNCTION();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void openglIndexBuffer::unbind() const {
    WEN_PROFILE_FUNCTION();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
}  // namespace wen