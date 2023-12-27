#include "wen/renderer/renderer.hpp"
#include "wen/renderer/renderer2D.hpp"
#include "wen/renderer/renderCommand.hpp"
#include "platform/opengl/openglShader.hpp"
#include "wen/debug/instrumentor.hpp"

namespace wen {
renderer::sceneData* renderer::m_SceneData = new renderer::sceneData;

void renderer::init() {
    WEN_PROFILE_FUNCTION();
    renderCommand::init();
    renderer2D::init();
}

void renderer::beginScene(orthographicCamera& camera) {
    m_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
}

void renderer::endScene() {}

void renderer::submit(const Ref<shader>& shader,
                      const Ref<vertexArray>& vertexArray,
                      const glm::mat4& transform) {
    shader->bind();
    std::dynamic_pointer_cast<openglShader>(shader)->uploadUniformMat4(
        "u_ViewProjection", m_SceneData->viewProjectionMatrix);
    std::dynamic_pointer_cast<openglShader>(shader)->uploadUniformMat4(
        "u_Transform", transform);
    vertexArray->bind();
    renderCommand::drawIndexed(vertexArray);
}

void renderer::onWindowResize(uint32_t width, uint32_t height) {
    renderCommand::setViewport(0, 0, width, height);
}

void renderer::shutdown() {
    renderer2D::shutdown();
}
}  // namespace wen