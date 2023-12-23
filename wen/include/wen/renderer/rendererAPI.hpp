#pragma once

#include "wen/renderer/vertexArray.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace wen {
class rendererAPI {
public:
    enum class API { None = 0, OpenGL = 1 };
    virtual void init() = 0;
    virtual void setClearColor(const glm::vec4 &color) = 0;
    virtual void clear() = 0;
    virtual void drawIndexed(
        const std::shared_ptr<vertexArray> &vertexArray) = 0;

    inline static API getAPI() { return s_API; }

    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width,
                             uint32_t height) = 0;

private:
    static API s_API;
};
}  // namespace wen