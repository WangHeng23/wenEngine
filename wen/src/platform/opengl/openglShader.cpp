#include "pch.hpp"
#include "platform/opengl/openglShader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <wen/core/log.hpp>

namespace wen {
static GLenum shaderTypeFromString(const std::string &type) {
    if (type == "vertex") {
        return GL_VERTEX_SHADER;
    }
    if (type == "fragment" || type == "pixel") {
        return GL_FRAGMENT_SHADER;
    }
    WEN_CORE_ASSERT(false, "Unknown shader type!");
    return 0;
}

openglShader::openglShader(const std::string &filepath) {
    std::string source = readFile(filepath);
    auto ShaderSources = preProcess(source);
    compile(ShaderSources);
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = filepath.rfind('.');
    auto count = lastDot == std::string::npos ? filepath.size() - lastSlash
                                              : lastDot - lastSlash;
    m_Name = filepath.substr(lastSlash, count);
}

openglShader::openglShader(const std::string &name,
                           const std::string &vertexSrc,
                           const std::string &fragmentSrc)
    : m_Name(name) {
    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertexSrc;
    sources[GL_FRAGMENT_SHADER] = fragmentSrc;
    compile(sources);
}

openglShader::~openglShader() {
    glDeleteProgram(m_RendererID);
}

void openglShader::bind() const {
    glUseProgram(m_RendererID);
}

void openglShader::unbind() const {
    glUseProgram(0);
}

std::string openglShader::readFile(const std::string &filepath) {
    std::string result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in) {
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        if (size != -1) {
            result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
            in.close();
        } else {
            WEN_CORE_ERROR("Could not read from file '{0}'", filepath);
        }
    } else {
        WEN_CORE_ERROR("Could not open file '{0}'", filepath);
    }
    return result;
}

std::unordered_map<GLenum, std::string> openglShader::preProcess(
    const std::string &source) {
    std::unordered_map<GLenum, std::string> shaderSources;
    const char *typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);
    while (pos != std::string::npos) {
        size_t eol = source.find_first_of("\r\n", pos);
        WEN_CORE_ASSERT(eol != std::string::npos, "Syntax error");
        size_t begin = pos + typeTokenLength + 1;
        std::string type = source.substr(begin, eol - begin);
        WEN_CORE_ASSERT(shaderTypeFromString(type),
                        "Invalid shader type specified");
        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        WEN_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
        pos = source.find(typeToken, nextLinePos);
        shaderSources[shaderTypeFromString(type)] =
            source.substr(nextLinePos, pos - (nextLinePos == std::string::npos
                                                  ? source.size() - 1
                                                  : nextLinePos));
    }
    return shaderSources;
}

void openglShader::compile(
    const std::unordered_map<GLenum, std::string> &shaderSources) {
    GLuint program = glCreateProgram();
    WEN_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders");
    std::array<GLenum, 2> glShaderIDs;
    int glShaderIDIndex = 0;
    for (auto &kv : shaderSources) {
        GLenum type = kv.first;
        const std::string &source = kv.second;
        GLuint shader = glCreateShader(type);
        const GLchar *sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
            glDeleteShader(shader);
            WEN_CORE_ERROR("{0}", infoLog.data());
            WEN_CORE_ASSERT(false, "Shader compilation failure!");
            break;
        }

        glAttachShader(program, shader);
        glShaderIDs[glShaderIDIndex++] = shader;
    }

    m_RendererID = program;
    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        glDeleteProgram(program);

        for (auto id : glShaderIDs) {
            glDeleteShader(id);
        }

        WEN_CORE_ERROR("{0}", infoLog.data());
        WEN_CORE_ASSERT(false, "Shader link failure!");
        return;
    }

    for (auto id : glShaderIDs) {
        glDetachShader(program, id);
        glDeleteShader(id);
    }
}

void openglShader::uploadUniformInt(const std::string &name, int value) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
}

void openglShader::uploadUniformFloat(const std::string &name, float value) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
}

void openglShader::uploadUniformFloat2(const std::string &name,
                                       const glm::vec2 &vector) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, vector.x, vector.y);
}

void openglShader::uploadUniformFloat3(const std::string &name,
                                       const glm::vec3 &vector) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void openglShader::uploadUniformFloat4(const std::string &name,
                                       const glm::vec4 &vector) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void openglShader::uploadUniformMat3(const std::string &name,
                                     const glm::mat3 &matrix) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void openglShader::uploadUniformMat4(const std::string &name,
                                     const glm::mat4 &matrix) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void openglShader::setInt(const std::string &name, const int value) {
    uploadUniformInt(name, value);
}

void openglShader::setFloat3(const std::string &name, const glm::vec3 &value) {
    uploadUniformFloat3(name, value);
}

void openglShader::setFloat4(const std::string &name, const glm::vec4 &value) {
    uploadUniformFloat4(name, value);
}

void openglShader::setMat4(const std::string &name, const glm::mat4 &value) {
    uploadUniformMat4(name, value);
}
}  // namespace wen