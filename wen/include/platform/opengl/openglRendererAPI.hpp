#pragma once 
#include "wen/renderer/rendererAPI.hpp"
#include "wen/core/base.hpp"

namespace wen {
class openglRendererAPI : public rendererAPI {
    void setClearColor(const glm::vec4& color) override;
    void clear() override;
    void drawIndexed(const Ref<vertexArray>& vertexArray) override;
    void init() override;
    void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
};
}