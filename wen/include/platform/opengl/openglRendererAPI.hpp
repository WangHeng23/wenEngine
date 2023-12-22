#pragma once 
#include "wen/renderer/rendererAPI.hpp"

namespace wen {
class openglRendererAPI : public rendererAPI {
    virtual void setClearColor(const glm::vec4& color) override;
    virtual void clear() override;
    virtual void drawIndexed(const std::shared_ptr<vertexArray>& vertexArray) override;
};
}