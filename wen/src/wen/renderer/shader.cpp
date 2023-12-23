#include "pch.hpp"
#include "wen/renderer/shader.hpp"
#include "wen/core/base.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "platform/opengl/openglShader.hpp"
#include "wen/renderer/renderer.hpp"
#include "wen/renderer/rendererAPI.hpp"

namespace wen {
Ref<shader> shader::create(const std::string& filepath) {
    switch (renderer::getAPI()) {
        case rendererAPI::API::None:
            WEN_CORE_ASSERT(false,
                            "rendererAPI::None is currently not supported!");
            return nullptr;
        case rendererAPI::API::OpenGL:
            return std::make_shared<openglShader>(filepath);
    }
    WEN_CORE_ASSERT(false, "Unknown rendererAPI!");
    return nullptr;
}

Ref<shader> shader::create(const std::string& name,
                           const std::string& vertexSrc,
                           const std::string& fragmentSrc) {
    switch (renderer::getAPI()) {
        case rendererAPI::API::None:
            WEN_CORE_ASSERT(false,
                            "rendererAPI::None is currently not supported!");
            return nullptr;
        case rendererAPI::API::OpenGL:
            return std::make_shared<openglShader>(name, vertexSrc, fragmentSrc);
    }
    WEN_CORE_ASSERT(false, "Unknown rendererAPI!");
    return nullptr;
}

void shaderLibrary::add(const std::string& name, const Ref<shader>& shader) {
    WEN_CORE_ASSERT(!exists(name), "shader already exists!");
    m_Shaders[name] = shader;
}

void shaderLibrary::add(const Ref<shader>& shader) {
    auto& name = shader->getName();
    WEN_CORE_ASSERT(!exists(name), "shader already exists!");
    add(name, shader);
}

Ref<shader> shaderLibrary::load(const std::string& filepath) {
    auto shader = shader::create(filepath);
    add(shader);
    return shader;
}

Ref<shader> shaderLibrary::load(const std::string& name,
                                const std::string& filepath) {
    auto shader = shader::create(filepath);
    add(shader);
    return shader;
}

Ref<shader> shaderLibrary::get(const std::string& name) const {
    WEN_CORE_ASSERT(exists(name), "shader not found!");
    return m_Shaders.at(name);
}

bool shaderLibrary::exists(const std::string& name) const {
    return m_Shaders.find(name) != m_Shaders.end();
}
}  // namespace wen