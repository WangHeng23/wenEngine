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

    static void flush();

    static void drawQuad(const glm::vec2& position, const glm::vec2& size,
                         const glm::vec4& color);
    static void drawQuad(const glm::vec3& position, const glm::vec2& size,
                         const glm::vec4& color);
    static void drawQuad(const glm::vec2& position, const glm::vec2& size,
                         const Ref<texture2D>& texture,
                         float tilingFactor = 1.0f,
                         const glm::vec4& tintColor = glm::vec4(1.0f));
    static void drawQuad(const glm::vec3& position, const glm::vec2& size,
                         const Ref<texture2D>& texture,
                         float tilingFactor = 1.0f,
                         const glm::vec4& tintColor = glm::vec4(1.0f));

    static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size,
                         float rotation, const glm::vec4& color);
    static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size,
                         float rotation, const glm::vec4& color);
    static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size,
                         float rotation, const Ref<texture2D>& texture,
                         float tilingFactor = 1.0f,
                         const glm::vec4& tintColor = glm::vec4(1.0f));
    static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size,
                         float rotation, const Ref<texture2D>& texture,
                         float tilingFactor = 1.0f,
                         const glm::vec4& tintColor = glm::vec4(1.0f));

    struct statistics {
        uint32_t drawCalls = 0;
        uint32_t quadCount = 0;

        uint32_t getTotalVertexCount() { return quadCount * 4; }

        uint32_t getTotalIndexCount() { return quadCount * 6; }
    };
    static void resetStats();
    static statistics getStats();

    static void FlushAndReset();

private:
};
}  // namespace wen