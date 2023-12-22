#include "wen/renderer/renderer.hpp"

namespace wen {
void renderer::beginScene() {}

void renderer::endScene() {}

void renderer::submit(const std::shared_ptr<vertexArray> &vertexArray) {
    vertexArray->bind();
    rendererCommand::drawIndexed(vertexArray);
}
}