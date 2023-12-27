#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <wen/core/base.hpp>

namespace wen {
class shader {
public:
    ~shader() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual const std::string& getName() const = 0;
    virtual void setInt(const std::string& name, const int value) = 0;
    virtual void setMat4(const std::string& name, const glm::mat4& value) = 0;
    virtual void setFloat(const std::string& name, const float value) = 0;
    virtual void setFloat3(const std::string& name, const glm::vec3& value) = 0;
    virtual void setFloat4(const std::string& name, const glm::vec4& value) = 0;

    static Ref<shader> create(const std::string& filepath);
    static Ref<shader> create(const std::string& name,
                              const std::string& vertexSrc,
                              const std::string& fragmentSrc);
};

class shaderLibrary {
public:
    void add(const std::string& name, const Ref<shader>& shader);
    void add(const Ref<shader>& shader);
    Ref<shader> load(const std::string& filepath);
    Ref<shader> load(const std::string& name, const std::string& filepath);
    Ref<shader> get(const std::string& name) const;
    bool exists(const std::string& name) const;

private:
    std::unordered_map<std::string, Ref<shader>> m_Shaders;
};
}  // namespace wen