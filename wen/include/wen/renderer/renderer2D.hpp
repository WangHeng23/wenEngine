#pragma once

#include "wen/renderer/orthographicCamera.hpp"
#include "wen/renderer/texture.hpp"

namespace wen {
class renderer2D {
public:
    static void init();
    static void shutdown();

    static void beginScene(const orthographicCamera& camera);
    static void endScene();

    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<texture2D>& texture);
    static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<texture2D>& texture);
};
}