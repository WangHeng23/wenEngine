#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace wen {
enum class shaderDataType {
    none = 0,
    float1,
    float2,
    float3,
    float4,
    mat3,
    mat4,
    int1,
    int2,
    int3,
    int4,
    bool1
};

static uint32_t shaderDataTypeSize(shaderDataType type) {
    switch (type) {
        case shaderDataType::float1:
            return 4;
        case shaderDataType::float2:
            return 4 * 2;
        case shaderDataType::float3:
            return 4 * 3;
        case shaderDataType::float4:
            return 4 * 4;
        case shaderDataType::mat3:
            return 4 * 3 * 3;
        case shaderDataType::mat4:
            return 4 * 4 * 4;
        case shaderDataType::int1:
            return 4;
        case shaderDataType::int2:
            return 4 * 2;
        case shaderDataType::int3:
            return 4 * 3;
        case shaderDataType::int4:
            return 4 * 4;
        case shaderDataType::bool1:
            return 1;
        default:
            return 0;
    }
}

struct bufferElement {
    std::string name;
    shaderDataType type;
    uint32_t size;
    uint32_t offset;
    bool normalized;

    bufferElement() = default;

    bufferElement(shaderDataType type, const std::string &name,
                  bool normalized = false)
        : name(name),
          type(type),
          size(shaderDataTypeSize(type)),
          offset(0),
          normalized(normalized) {}

    uint32_t getComponentCount() const {
        switch (type) {
            case shaderDataType::float1:
                return 1;
            case shaderDataType::float2:
                return 2;
            case shaderDataType::float3:
                return 3;
            case shaderDataType::float4:
                return 4;
            case shaderDataType::mat3:
                return 3 * 3;
            case shaderDataType::mat4:
                return 4 * 4;
            case shaderDataType::int1:
                return 1;
            case shaderDataType::int2:
                return 2;
            case shaderDataType::int3:
                return 3;
            case shaderDataType::int4:
                return 4;
            case shaderDataType::bool1:
                return 1;
            default:
                return 0;
        }
    }
};

class bufferLayout {
public:
    bufferLayout() = default;

    bufferLayout(const std::initializer_list<bufferElement> &elements)
        : m_Elements(elements) {
        calculateOffsetsAndStride();
    }

    inline uint32_t getStride() const { return m_Stride; }

    inline const std::vector<bufferElement> &getElements() const {
        return m_Elements;
    }

    std::vector<bufferElement>::iterator begin() { return m_Elements.begin(); }

    std::vector<bufferElement>::iterator end() { return m_Elements.end(); }

    std::vector<bufferElement>::const_iterator begin() const {
        return m_Elements.begin();
    }

    std::vector<bufferElement>::const_iterator end() const {
        return m_Elements.end();
    }

private:
    void calculateOffsetsAndStride() {
        uint32_t offset = 0;
        m_Stride = 0;
        for (auto &element : m_Elements) {
            element.offset = offset;
            offset += element.size;
            m_Stride += element.size;
        }
    }

private:
    std::vector<bufferElement> m_Elements;
    uint32_t m_Stride = 0;
};

class vertexBuffer {
public:
    virtual ~vertexBuffer() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void setData(const void *data, uint32_t size) = 0;

    virtual void setLayout(const bufferLayout &layout) = 0;
    virtual const bufferLayout &getLayout() const = 0;

    static Ref<vertexBuffer> create(uint32_t size);
    static Ref<vertexBuffer> create(float *vertices, uint32_t size);
};

class indexBuffer {
public:
    virtual ~indexBuffer() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual uint32_t getCount() const = 0;
    static Ref<indexBuffer> create(uint32_t *indices, uint32_t count);
};
}  // namespace wen