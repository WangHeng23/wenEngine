#include "platform/opengl/openglTexture.hpp"
#include "wen/debug/instrumentor.hpp"

#include "stb_image.h"
#include <glad/glad.h>

namespace wen {
openglTexture2D::openglTexture2D(const std::string &path) : m_Path(path) {
    WEN_PROFILE_FUNCTION();
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc *data = nullptr;
    { data = stbi_load(path.c_str(), &width, &height, &channels, 0); }
    WEN_CORE_ASSERT(data, "Failed to load image!");
    m_Width = width;
    m_Height = height;

    GLenum internalFormat = 0, dataFormat = 0;
    if (channels == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    } else if (channels == 3) {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }
    WEN_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0,
                 dataFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

openglTexture2D::openglTexture2D(uint32_t w, uint32_t h)
    : m_Width(w), m_Height(h) {
    WEN_PROFILE_FUNCTION();
    m_InternalFormat = GL_RGBA8;
    m_DataFormat = GL_RGBA;
    
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, w, h, 0, m_DataFormat,
                 GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

openglTexture2D::~openglTexture2D() {
    WEN_PROFILE_FUNCTION();
    glDeleteTextures(1, &m_RendererID);
}

void openglTexture2D::bind(uint32_t slot) const {
    WEN_PROFILE_FUNCTION();
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void openglTexture2D::setData(void *data, uint32_t size) {
    WEN_PROFILE_FUNCTION();
    uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
    WEN_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire");

    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat,
                    GL_UNSIGNED_BYTE, data);
}
}  // namespace wen
