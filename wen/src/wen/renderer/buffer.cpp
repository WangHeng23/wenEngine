#include "wen/renderer/buffer.hpp"
#include "wen/renderer/renderer.hpp"

#include "platform/opengl/openglBuffer.hpp"

namespace wen {
Ref<vertexBuffer> vertexBuffer::create(uint32_t size) {
    switch (renderer::getAPI()) {
        case rendererAPI::API::None:
            WEN_CORE_ASSERT(
                false, "rendererAPI::API::None is currently not supported!");
            return nullptr;
        case rendererAPI::API::OpenGL:
            return CreateRef<openglVertexBuffer>(size);
    }

    WEN_CORE_ASSERT(false, "Unknown rendererAPI!");
    return nullptr;
}

Ref<vertexBuffer> vertexBuffer::create(float *vertices, uint32_t size) {
    switch (renderer::getAPI()) {
        case rendererAPI::API::None:
            WEN_CORE_ASSERT(
                false, "rendererAPI::API::None is currently not supported!");
            return nullptr;
        case rendererAPI::API::OpenGL:
            return CreateRef<openglVertexBuffer>(vertices, size);
    }

    WEN_CORE_ASSERT(false, "Unknown rendererAPI!");
    return nullptr;
}

Ref<indexBuffer> indexBuffer::create(uint32_t *indices, uint32_t count) {
    switch (renderer::getAPI()) {
        case rendererAPI::API::None:
            WEN_CORE_ASSERT(
                false, "rendererAPI::API::None is currently not supported!");
            return nullptr;
        case rendererAPI::API::OpenGL:
            return CreateRef<openglIndexBuffer>(indices, count);
    }

    WEN_CORE_ASSERT(false, "Unknown rendererAPI!");
    return nullptr;
}
}  // namespace wen