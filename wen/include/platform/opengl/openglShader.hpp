#pragma once

#include "wen/renderer/shader.hpp"
#include <unordered_map>

typedef unsigned int GLenum;

namespace wen {
class openglShader : public shader {
public:
    openglShader(const std::string& filepath);
    openglShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    ~openglShader();

    void bind() const override;
    void unbind() const override;

    void uploadUniformInt(const std::string& name, int value);
    void uploadUniformIntArray(const std::string& name, int* values, uint32_t count);
    void uploadUniformFloat(const std::string& name, float value);
    void uploadUniformFloat2(const std::string& name, const glm::vec2& vector);
    void uploadUniformFloat3(const std::string& name, const glm::vec3& vector);
    void uploadUniformFloat4(const std::string& name, const glm::vec4& vector);
    void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
    void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

    const std::string& getName() const override { return m_Name; }

    void setInt(const std::string& name, const int value) override;
    void setMat4(const std::string& name, const glm::mat4& value) override;
    void setFloat(const std::string& name, const float value) override;
    void setFloat3(const std::string& name, const glm::vec3& value) override;
    void setFloat4(const std::string& name, const glm::vec4& value) override;

    void setIntArray(const std::string& name, int* values, uint32_t count) override;

private:
    std::string readFile(const std::string& filepath);
    std::unordered_map<GLenum, std::string> preProcess(const std::string& source);
    void compile(const std::unordered_map<GLenum, std::string>& shaderSources);

private:
    uint32_t m_RendererID;
    std::string m_Name;
};
}  // namespace wen