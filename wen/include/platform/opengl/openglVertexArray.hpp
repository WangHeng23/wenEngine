#pragma once

#include "wen/renderer/vertexArray.hpp"
#include <cstdint>

namespace wen {
class openglVertexArray : public vertexArray {
public:
    openglVertexArray();
    virtual ~openglVertexArray();

    virtual void bind() const override;
    virtual void unbind() const override;

    virtual void addVertexBuffer(
        const std::shared_ptr<vertexBuffer> &vertexBuffer) override;
    virtual void setIndexBuffer(
    std::shared_ptr<indexBuffer> &indexBuffer) override;

    virtual const std::vector<std::shared_ptr<vertexBuffer>> &getVertexBuffers()
        const override {
        return m_VertexBuffers;
    }

    virtual const std::shared_ptr<indexBuffer> &getIndexBuffer()
        const override {
        return m_IndexBuffer;
    }

private:
    uint32_t m_RendererID;
    std::vector<std::shared_ptr<vertexBuffer>> m_VertexBuffers;
    std::shared_ptr<indexBuffer> m_IndexBuffer;
};
}  // namespace wen