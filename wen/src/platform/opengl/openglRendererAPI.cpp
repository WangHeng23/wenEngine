#include "platform/opengl/openglRendererAPI.hpp"

#include <glad/glad.h>

namespace wen {
void openglRendererAPI::setClearColor(const glm::vec4 &color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void openglRendererAPI::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void openglRendererAPI::drawIndexed(
    const std::shared_ptr<vertexArray> &vertexArray) {
    glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(),
                   GL_UNSIGNED_INT, nullptr);
}
}  // namespace wen