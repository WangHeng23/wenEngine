#include "wen/renderer/vertexArray.hpp"
#include "wen/renderer/renderer.hpp"
#include "platform/opengl/OpenGLVertexArray.hpp"

namespace wen {
vertexArray *vertexArray::create() {
    switch (renderer::getAPI()) {
        case rendererAPI::API::None:
            WEN_CORE_ASSERT(false,
                            "RendererAPI::none is currently not supported!");
            return nullptr;
        case rendererAPI::API::OpenGL:
            return new openglVertexArray();
    }

    WEN_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
}  // namespace wen