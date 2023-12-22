#include "wen/renderer/buffer.hpp"
#include "wen/renderer/renderer.hpp"

#include "platform/opengl/openglBuffer.hpp"

namespace wen {
vertexBuffer *vertexBuffer::create(float *vertices, uint32_t size) {
    switch (renderer::getAPI()) {
        case rendererAPI::API::None:
            WEN_CORE_ASSERT(
                false, "rendererAPI::API::None is currently not supported!");
            return nullptr;
        case rendererAPI::API::OpenGL:
            return new openglVertexBuffer(vertices, size);
    }

    WEN_CORE_ASSERT(false, "Unknown rendererAPI!");
    return nullptr;
}

indexBuffer *indexBuffer::create(uint32_t *indices, uint32_t count) {
    switch (renderer::getAPI()) {
        case rendererAPI::API::None:
            WEN_CORE_ASSERT(
                false, "rendererAPI::API::None is currently not supported!");
            return nullptr;
        case rendererAPI::API::OpenGL:
            return new openglIndexBuffer(indices, count);
    }

    WEN_CORE_ASSERT(false, "Unknown rendererAPI!");
    return nullptr;
}
}  // namespace wen