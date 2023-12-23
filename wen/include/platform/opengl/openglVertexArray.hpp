#pragma once

#include "wen/renderer/vertexArray.hpp"
#include "wen/core/base.hpp"
#include <cstdint>

namespace wen {
class openglVertexArray : public vertexArray {
public:
    openglVertexArray();
    virtual ~openglVertexArray();

    virtual void bind() const override;
    virtual void unbind() const override;

    virtual void addVertexBuffer(const Ref<vertexBuffer> &vertexBuffer) override;
    virtual void setIndexBuffer(Ref<indexBuffer> &indexBuffer) override;

    virtual const std::vector<Ref<vertexBuffer>> &getVertexBuffers()
        const override {
        return m_VertexBuffers;
    }

    virtual const Ref<indexBuffer> &getIndexBuffer() const override {
        return m_IndexBuffer;
    }

private:
    uint32_t m_RendererID;
    std::vector<Ref<vertexBuffer>> m_VertexBuffers;
    Ref<indexBuffer> m_IndexBuffer;
};
}  // namespace wen