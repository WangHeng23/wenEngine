#pragma once
#include "wen/renderer/rendererCommand.hpp"

namespace wen {
class renderer {
public:
    static void beginScene();
    static void endScene();

    static void submit(const std::shared_ptr<vertexArray> &vertexArray);

    inline static rendererAPI::API getAPI() { return rendererAPI::getAPI(); }
};
}  // namespace wen