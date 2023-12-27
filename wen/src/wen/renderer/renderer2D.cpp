#include "wen/renderer/renderer2D.hpp"
#include "wen/renderer/vertexArray.hpp"
#include "wen/renderer/shader.hpp"
#include "wen/renderer/renderCommand.hpp"
#include "wen/debug/instrumentor.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace wen {
struct quadVertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoord;
};

struct renderer2DData {
    const uint32_t maxQuads = 10000;
    const uint32_t maxVertices = maxQuads * 4;
    const uint32_t maxIndices = maxQuads * 6;
    Ref<vertexArray> quadVertexArray;
    Ref<vertexBuffer> quadVertexBuffer;
    Ref<shader> textureShader;
    Ref<texture2D> whiteTexture;

    uint32_t quadIndexCount = 0;
    quadVertex* quadVertexBufferBase = nullptr;
    quadVertex* quadVertexBufferPtr = nullptr;
};

static renderer2DData* s_Data;

void renderer2D::init() {
    WEN_PROFILE_FUNCTION();
    s_Data = new renderer2DData();
    s_Data->quadVertexArray = vertexArray::create();
    float squareVertices[5 * 4] = {-0.5, -0.5f, 0.0f, 0.0f, 0.0f, 0.5,  -0.5f,
                                   0.0f, 1.0f,  0.0f, 0.5,  0.5f, 0.0f, 1.0f,
                                   1.0f, -0.5,  0.5f, 0.0f, 0.0f, 1.0f};
    Ref<vertexBuffer> squareVB;
    squareVB.reset(
        vertexBuffer::create(squareVertices, sizeof(squareVertices)));
    squareVB->setLayout({
        {shaderDataType::float3, "a_Position"},
        {shaderDataType::float2, "a_TexCoord"}
    });
    s_Data->quadVertexArray->addVertexBuffer(squareVB);

    uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
    Ref<indexBuffer> squareIB;
    squareIB.reset(indexBuffer::create(
        squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    s_Data->quadVertexArray->setIndexBuffer(squareIB);

    s_Data->whiteTexture = texture2D::create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data->whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

    s_Data->textureShader =
        shader::create("sandbox/assets/shaders/GameTexture.glsl");
    s_Data->textureShader->bind();
    s_Data->textureShader->setInt("u_Texture", 0);
}

void renderer2D::shutdown() {
    WEN_PROFILE_FUNCTION();
    delete s_Data;
}

void renderer2D::beginScene(const orthographicCamera& camera) {
    WEN_PROFILE_FUNCTION();
    s_Data->textureShader->bind();
    s_Data->textureShader->setMat4("u_ViewProjection",
                                   camera.getViewProjectionMatrix());
}

void renderer2D::endScene() {
    WEN_PROFILE_FUNCTION();
}

void renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size,
                          const glm::vec4& color) {
    WEN_PROFILE_FUNCTION();
    drawQuad({position.x, position.y, 0.0f}, size, color);
}

void renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size,
                          const glm::vec4& color) {
    WEN_PROFILE_FUNCTION();
    s_Data->textureShader->setFloat4("u_Color", color);
    s_Data->textureShader->setFloat("u_TilingFactor", 1.0f);
    s_Data->whiteTexture->bind();
    s_Data->textureShader->setMat4(
        "u_Transform", glm::translate(glm::mat4(1.0f), position) *
                           glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f}));
    s_Data->quadVertexArray->bind();
    renderCommand::drawIndexed(s_Data->quadVertexArray);
}

void renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size,
                          const Ref<texture2D>& texture, float tilingFactor,
                          const glm::vec4& tintColor) {
    WEN_PROFILE_FUNCTION();
    drawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor,
             tintColor);
}

void renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size,
                          const Ref<texture2D>& texture, float tilingFactor,
                          const glm::vec4& tintColor) {
    WEN_PROFILE_FUNCTION();
    s_Data->textureShader->bind();
    s_Data->textureShader->setFloat4("u_Color", glm::vec4(1.0f));
    s_Data->textureShader->setFloat("u_TilingFactor", tilingFactor);
    texture->bind();
    s_Data->textureShader->setMat4(
        "u_Transform", glm::translate(glm::mat4(1.0f), position) *
                           glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f}));
    s_Data->quadVertexArray->bind();
    renderCommand::drawIndexed(s_Data->quadVertexArray);
}

void renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size,
                          float rotation, const glm::vec4& color) {
    WEN_PROFILE_FUNCTION();
    drawQuad({position.x, position.y, 0.0f}, size, rotation, color);
}

void renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size,
                          float rotation, const glm::vec4& color) {
    WEN_PROFILE_FUNCTION();
    s_Data->textureShader->setFloat4("u_Color", color);
    s_Data->textureShader->setFloat("u_TilingFactor", 1.0f);
    s_Data->whiteTexture->bind();
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) *
        glm::rotate(glm::mat4(1.0), rotation, {0.0f, 0.0f, 1.0f}) *
        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
    s_Data->textureShader->setMat4("u_Transform", transform);
    renderCommand::drawIndexed(s_Data->quadVertexArray);
}

void renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size,
                          float rotation, const Ref<texture2D>& texture,
                          float tilingFactor, const glm::vec4& tintColor) {
    WEN_PROFILE_FUNCTION();
    drawQuad({position.x, position.y, 0.0f}, size, rotation, texture,
             tilingFactor, tintColor);
}

void renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size,
                          float rotation, const Ref<texture2D>& texture,
                          float tilingFactor, const glm::vec4& tintColor) {
    WEN_PROFILE_FUNCTION();
    s_Data->textureShader->setFloat4("u_Color", tintColor);
    s_Data->textureShader->setFloat("u_TilingFactor", tilingFactor);
    texture->bind();
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) *
        glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) *
        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
    s_Data->textureShader->setMat4("u_Transform", transform);
    s_Data->quadVertexArray->bind();
    renderCommand::drawIndexed(s_Data->quadVertexArray);
}
}  // namespace wen