#pragma once

#include "wen/renderer/rendererAPI.hpp"
#include "wen/renderer/vertexArray.hpp"

namespace wen {
class rendererCommand {
public:
    static inline void setClearColor(const glm::vec4 &color) {
        s_RendererAPI->setClearColor(color);
    }

    static inline void clear() { s_RendererAPI->clear(); }

    static inline void drawIndexed(
        const std::shared_ptr<vertexArray> &vertexArray) {
        s_RendererAPI->drawIndexed(vertexArray);
    }

private:
    static rendererAPI *s_RendererAPI;
};
}  // namespace wen