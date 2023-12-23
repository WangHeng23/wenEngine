#include "wen/renderer/texture.hpp"

#include "wen/renderer/renderer.hpp"
#include "wen/renderer/rendererAPI.hpp"
#include "platform/opengl/openglTexture.hpp"

namespace wen {
Ref<texture2D> texture2D::create(const std::string &path) {
    switch (renderer::getAPI()) {
        case rendererAPI::API::None:
            WEN_CORE_ASSERT(false,
                            "rendererAPI::None is currently not supported!");
            return nullptr;
        case rendererAPI::API::OpenGL:
            return CreateRef<openglTexture2D>(path);
    }

    WEN_CORE_ASSERT(false, "Unknown rendererAPI!");
    return nullptr;
}

Ref<texture2D> texture2D::create(uint32_t w, uint32_t h) {
    switch (renderer::getAPI()) {
        case rendererAPI::API::None:
            WEN_CORE_ASSERT(false,
                            "rendererAPI::None is currently not supported!");
            return nullptr;
        case rendererAPI::API::OpenGL:
            return CreateRef<openglTexture2D>(w, h);
    }

    WEN_CORE_ASSERT(false, "Unknown rendererAPI!");
    return nullptr;
}
}  // namespace wen