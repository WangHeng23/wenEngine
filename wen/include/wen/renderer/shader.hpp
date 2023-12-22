#pragma once

#include <string>

namespace wen {
class shader {
public:
    shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~shader(); 

    void bind() const;
    void unbind() const;

private:
    uint32_t m_RendererID;
};
}