#pragma once

#include <string>
#include <cstdint>
#include "wen/core/base.hpp"

namespace wen {
class texture {
public:
    virtual ~texture() = default;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;

    virtual void setData(void* data, uint32_t size) = 0;
    virtual void bind(uint32_t slot = 0) const = 0;
};

class texture2D : public texture {
public:
    static Ref<texture2D> create(const std::string& path);
    static Ref<texture2D> create(uint32_t width, uint32_t height);

    virtual bool operator==(const texture2D& other) const = 0;
};
}