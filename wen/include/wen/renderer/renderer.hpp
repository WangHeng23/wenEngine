#pragma once
#include "wen/renderer/rendererAPI.hpp"
#include "wen/renderer/orthographicCamera.hpp"
#include "wen/renderer/shader.hpp"
#include "wen/core/base.hpp"

namespace wen {
class renderer {
public:
    static void init();
    static void shutdown();
    static void beginScene(orthographicCamera &camera);
    static void endScene();

    static void submit(const Ref<shader> &shadar,
                       const Ref<vertexArray> &vertexArray,
                       const glm::mat4 &transform = glm::mat4(1.0f));

    static void onWindowResize(uint32_t width, uint32_t height);

    inline static rendererAPI::API getAPI() { return rendererAPI::getAPI(); }

    struct sceneData {
        glm::mat4 viewProjectionMatrix;
    };

    static sceneData *m_SceneData;
};
}  // namespace wen