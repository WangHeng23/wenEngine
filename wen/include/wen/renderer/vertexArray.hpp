#pragma once 

#include <memory>   

#include "wen/renderer/buffer.hpp"

namespace wen {
class vertexArray {
public:
    virtual ~vertexArray() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(const std::shared_ptr<vertexBuffer> &vertexBuffer) = 0;
    virtual void setIndexBuffer(std::shared_ptr<indexBuffer> &indexBuffer) = 0;

    virtual const std::vector<std::shared_ptr<vertexBuffer>> &getVertexBuffers() const = 0;
    virtual const std::shared_ptr<indexBuffer> &getIndexBuffer() const = 0;

    static vertexArray *create();
};
}