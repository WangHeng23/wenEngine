#pragma once

#include <string>
#include "wen/renderer/texture.hpp"
#include <glad/glad.h>

namespace wen {
class openglTexture2D : public texture2D {
public:
    explicit openglTexture2D(const std::string& path);
    openglTexture2D(uint32_t w, uint32_t h);
    ~openglTexture2D() override;

    uint32_t getWidth() const override { return m_Width; }
    uint32_t getHeight() const override { return m_Height; }

    void setData(void* data, uint32_t size) override;
    void bind(uint32_t slot = 0) const override;

private:
    uint32_t m_RendererID;
    std::string m_Path;
    uint32_t m_Width, m_Height;
    GLint m_InternalFormat = 0;
    GLenum m_DataFormat = 0;
};
}  // namespace wen