#pragma once

#include "wen/renderer/rendererAPI.hpp"
#include "wen/renderer/vertexArray.hpp"

namespace wen {
class renderCommand {
public:
    inline static void setViewport(uint32_t x, uint32_t y, uint32_t width,
                                   uint32_t height) {
        s_RendererAPI->setViewport(x, y, width, height);
    }

    static inline void setClearColor(const glm::vec4 &color) {
        s_RendererAPI->setClearColor(color);
    }

    static inline void clear() { s_RendererAPI->clear(); }

    static inline void drawIndexed(
        const std::shared_ptr<vertexArray> &vertexArray) {
        s_RendererAPI->drawIndexed(vertexArray);
    }

    static void init() { s_RendererAPI->init(); }

private:
    static rendererAPI *s_RendererAPI;
};
}  // namespace wen