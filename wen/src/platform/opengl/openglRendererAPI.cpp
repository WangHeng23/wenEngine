#include "platform/opengl/openglRendererAPI.hpp"
#include "wen/debug/instrumentor.hpp"

#include <glad/glad.h>

namespace wen {
void openglRendererAPI::setClearColor(const glm::vec4 &color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void openglRendererAPI::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void openglRendererAPI::drawIndexed(const Ref<vertexArray> &vertexArray,
                                    uint32_t indexCount) {
    uint32_t count =
        indexCount ? indexCount : vertexArray->getIndexBuffer()->getCount();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void openglRendererAPI::init() {
    WEN_PROFILE_FUNCTION();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void openglRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width,
                                    uint32_t height) {
    glViewport(x, y, width, height);
}
}  // namespace wen