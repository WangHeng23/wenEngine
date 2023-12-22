#pragma once

#include "wen/renderer/buffer.hpp"

namespace wen {
class openglVertexBuffer : public vertexBuffer {
public:
    openglVertexBuffer(float *vertices, uint32_t size);
    virtual ~openglVertexBuffer();

    void bind() const override;
    void unbind() const override;

    void setLayout(const bufferLayout &layout) override { m_Layout = layout; }

    const bufferLayout &getLayout() const override { return m_Layout; }

private:
    uint32_t m_RendererID;
    bufferLayout m_Layout;
};

class openglIndexBuffer : public indexBuffer {
public:
    openglIndexBuffer(uint32_t *indices, uint32_t count);
    virtual ~openglIndexBuffer();

    virtual void bind() const override;
    virtual void unbind() const override;

    virtual uint32_t getCount() const override { return m_Count; }

private:
    uint32_t m_RendererID;
    uint32_t m_Count;
};
}  // namespace wen