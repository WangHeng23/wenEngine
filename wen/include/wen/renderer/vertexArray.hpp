#pragma once 

#include <memory>   

#include "wen/renderer/buffer.hpp"
#include "wen/core/base.hpp"

namespace wen {
class vertexArray {
public:
    virtual ~vertexArray() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(const Ref<vertexBuffer> &vertexBuffer) = 0;
    virtual void setIndexBuffer(Ref<indexBuffer> &indexBuffer) = 0;

    virtual const std::vector<Ref<vertexBuffer>> &getVertexBuffers() const = 0;
    virtual const Ref<indexBuffer> &getIndexBuffer() const = 0;

    static Ref<vertexArray> create();
};
}  // namespace wen