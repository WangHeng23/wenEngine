#include "pch.hpp"
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
    float texIndex;
    float tilingFactor;
};

struct renderer2DData {
    // 一个批次最多绘制10000个四边形
    const uint32_t maxQuads = 10000;
    // 一个批次最多绘制10000 * 4个顶点
    const uint32_t maxVertices = maxQuads * 4;
    // 一个批次最多绘制10000 * 6个索引 
    const uint32_t maxIndices = maxQuads * 6;
    // 最大纹理槽数
    static const uint32_t maxTextureSlots = 32;

    // vertexArray
    Ref<vertexArray> quadVertexArray;
    // vertexBuffer
    Ref<vertexBuffer> quadVertexBuffer;
    // shader
    Ref<shader> textureShader;
    // texture
    Ref<texture2D> whiteTexture;

    // 索引总数量
    uint32_t quadIndexCount = 0;
    // 顶点数据的起始地址  
    quadVertex* quadVertexBufferBase = nullptr;
    // 顶点数据的当前地址
    quadVertex* quadVertexBufferPtr = nullptr;

    std::array<Ref<texture2D>, maxTextureSlots> textureSlots;
    uint32_t textureSlotIndex = 1;  // 0 = whiteTexture
};

static renderer2DData s_Data;

void renderer2D::init() {
    WEN_PROFILE_FUNCTION();
    s_Data.quadVertexArray = vertexArray::create();
    s_Data.quadVertexBuffer = vertexBuffer::create(s_Data.maxVertices * sizeof(quadVertex));
    s_Data.quadVertexBuffer->setLayout({
        {shaderDataType::float3, "a_Position"},
        {shaderDataType::float4, "a_Color"},
        {shaderDataType::float2, "a_TexCoord"},
        {shaderDataType::float1, "a_TexIndex"},
        {shaderDataType::float1, "a_TilingFactor"},
    });
    s_Data.quadVertexArray->addVertexBuffer(s_Data.quadVertexBuffer);
    s_Data.quadVertexBufferBase = new quadVertex[s_Data.maxVertices];

    uint32_t* quadIndices = new uint32_t[s_Data.maxIndices];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_Data.maxIndices; i += 6) {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }

    Ref<indexBuffer> quadIndexBuffer = indexBuffer::create(quadIndices, s_Data.maxIndices);
    s_Data.quadVertexArray->setIndexBuffer(quadIndexBuffer);
    delete[] quadIndices;
    
    s_Data.whiteTexture = texture2D::create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));
    s_Data.textureSlots[0] = s_Data.whiteTexture;

    s_Data.textureShader =
        shader::create("sandbox/assets/shaders/Texture.glsl");
    int samplers[s_Data.maxTextureSlots] = {0};
    for (int i = 0; i < s_Data.maxTextureSlots; i++) {
        samplers[i] = i;
    }
    s_Data.textureShader->bind();
    s_Data.textureShader->setIntArray("u_Textures", samplers,
                                      s_Data.maxTextureSlots);
}

void renderer2D::shutdown() {
    WEN_PROFILE_FUNCTION();
    delete[] s_Data.quadVertexBufferBase;
}

void renderer2D::beginScene(const orthographicCamera& camera) {
    WEN_PROFILE_FUNCTION();
    s_Data.textureShader->bind();
    s_Data.textureShader->setMat4("u_ViewProjection",
                                  camera.getViewProjectionMatrix());
    s_Data.quadIndexCount = 0;
    s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;
}

void renderer2D::endScene() {
    WEN_PROFILE_FUNCTION();
    uint32_t dataSize = (uint8_t*)s_Data.quadVertexBufferPtr -
                        (uint8_t*)s_Data.quadVertexBufferBase;
    s_Data.quadVertexBuffer->setData(s_Data.quadVertexBufferBase, dataSize);
    flush();
}

void renderer2D::flush() {
    WEN_PROFILE_FUNCTION();
    for (uint32_t i = 0; i < s_Data.textureSlotIndex; i++) {
        s_Data.textureSlots[i]->bind(i);
    }
    renderCommand::drawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);
}

void renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size,
                          const glm::vec4& color) {
    WEN_PROFILE_FUNCTION();
    drawQuad({position.x, position.y, 0.0f}, size, color);
}

void renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size,
                          const glm::vec4& color) {
    WEN_PROFILE_FUNCTION();
    const float textureIndex = 0.0f;  // white texture
    const float tilingFactor = 1.0f;

    s_Data.quadVertexBufferPtr->position = position;
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = {0.0f, 0.0f};
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = {position.x + size.x, position.y,
                                            0.0f};
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = {1.0f, 0.0f};
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = {position.x + size.x,
                                            position.y + size.y, 0.0f};
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = {1.0f, 1.0f};
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = {position.x, position.y + size.y,
                                            0.0f};
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = {0.0f, 1.0f};
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadIndexCount += 6;
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
    constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++) {
        if (*s_Data.textureSlots[i].get() == *texture.get()) {
            textureIndex = (float)i;
            break;
        }
    }
    if (textureIndex == 0.0f) {
        textureIndex = (float)s_Data.textureSlotIndex;
        s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
        s_Data.textureSlotIndex++;
    }
    s_Data.quadVertexBufferPtr->position = position;
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = {0.0f, 0.0f};
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = {position.x + size.x, position.y,
                                            0.0f};
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = {1.0f, 0.0f};
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = {position.x + size.x,
                                            position.y + size.y, 0.0f};
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = {1.0f, 1.0f};
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = {position.x, position.y + size.y,
                                            0.0f};
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = {0.0f, 1.0f};
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadIndexCount += 6;

    // s_Data.textureShader->setFloat4("u_Color", tintColor);
    // s_Data.textureShader->setFloat("u_TilingFactor", tilingFactor);
    // texture->bind();
    // s_Data.textureShader->setMat4(
    //     "u_Transform", glm::translate(glm::mat4(1.0f), position) *
    //                        glm::scale(glm::mat4(1.0f), {size.x,
    //                        size.y, 1.0f}));
    // s_Data.quadVertexArray->bind();
    // renderCommand::drawIndexed(s_Data.quadVertexArray);
}

void renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size,
                          float rotation, const glm::vec4& color) {
    WEN_PROFILE_FUNCTION();
    drawQuad({position.x, position.y, 0.0f}, size, rotation, color);
}

void renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size,
                          float rotation, const glm::vec4& color) {
    WEN_PROFILE_FUNCTION();
    s_Data.textureShader->setFloat4("u_Color", color);
    s_Data.textureShader->setFloat("u_TilingFactor", 1.0f);
    s_Data.whiteTexture->bind();
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) *
        glm::rotate(glm::mat4(1.0), rotation, {0.0f, 0.0f, 1.0f}) *
        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
    s_Data.textureShader->setMat4("u_Transform", transform);
    renderCommand::drawIndexed(s_Data.quadVertexArray);
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
    s_Data.textureShader->setFloat4("u_Color", tintColor);
    s_Data.textureShader->setFloat("u_TilingFactor", tilingFactor);
    texture->bind();
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) *
        glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) *
        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
    s_Data.textureShader->setMat4("u_Transform", transform);
    s_Data.quadVertexArray->bind();
    renderCommand::drawIndexed(s_Data.quadVertexArray);
}
}  // namespace wen