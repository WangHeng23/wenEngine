#include "exampleLayer.hpp"
#include "pch.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "platform/opengl/openglShader.hpp"

exampleLayer::exampleLayer()
    : layer("example"), m_CameraController(1600.f / 900.f, true) {
    // -----------triangle------------
    // 创建顶点数组
    m_VertexArray = (wen::vertexArray::create());
    float vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                             0.5f,  -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                             0.0f,  0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f};
    // 创建顶点缓冲区
    wen::Ref<wen::vertexBuffer> vertexBuffer_(
        wen::vertexBuffer::create(vertices, sizeof(vertices)));
    // 创建顶点缓冲区布局
    wen::bufferLayout layout = {
        {wen::shaderDataType::float3, "a_Position"},
        {wen::shaderDataType::float4,    "a_Color"}
    };
    // 设置顶点缓冲区布局
    vertexBuffer_->setLayout(layout);
    // 将顶点缓冲区添加到顶点数组
    m_VertexArray->addVertexBuffer(vertexBuffer_);
    uint32_t indices[3] = {0, 1, 2};
    // 创建索引缓冲区
    wen::Ref<wen::indexBuffer> indexBuffer_(
        wen::indexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
    // 将索引缓冲区添加到顶点数组
    m_VertexArray->setIndexBuffer(indexBuffer_);
    // ------------------------------

    // ----------square--------------
    m_SquareVA = (wen::vertexArray::create());
    float squareVertices[5 * 4] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f, -0.5f,
                                   0.0f,  1.0f,  0.0f, 0.5f, 0.5f, 0.0f, 1.0f,
                                   1.0f,  -0.5f, 0.5f, 0.0f, 0.0f, 1.0f};

    wen::Ref<wen::vertexBuffer> squareVB(
        wen::vertexBuffer::create(squareVertices, sizeof(squareVertices)));
    squareVB->setLayout({
        {wen::shaderDataType::float3, "a_Position"},
        {wen::shaderDataType::float2, "a_TexCoord"}
    });
    m_SquareVA->addVertexBuffer(squareVB);
    uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
    wen::Ref<wen::indexBuffer> squareIB(wen::indexBuffer::create(
        squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_SquareVA->setIndexBuffer(squareIB);
    // ------------------------------

    std::string vertexSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec4 a_Color;
    uniform mat4 u_ViewProjection;
    uniform mat4 u_Transform;
    out vec3 v_Position;
    out vec4 v_Color;
    void main()
    {
        v_Position = a_Position;
        v_Color = a_Color;
        gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    }
    )";

    std::string fragmentSrc = R"(
    #version 330 core
    layout(location = 0) out vec4 color;
    in vec3 v_Position;
    in vec4 v_Color;
    void main()
    {
        color = vec4(v_Position * 0.5 + 0.5, 1.0);
        color = v_Color;
    }
    )";

    m_Shader = wen::shader::create("VertexPosColor", vertexSrc, fragmentSrc);

    std::string FlatColorShaderVertexSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 a_Position;
    uniform mat4 u_ViewProjection;
    uniform mat4 u_Transform;
    out vec3 v_Position;
    void main()
    {
        v_Position = a_Position;
        gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    }
    )";
    std::string FlatColorShaderFragmentSrc = R"(
    #version 330 core
    layout(location = 0) out vec4 color;
    in vec3 v_Position;
    uniform vec3 u_Color;
    void main()
    {
        color = vec4(u_Color, 1.0);
    }
    )";
    m_FlatColorShader = wen::shader::create(
        "FlatColor", FlatColorShaderVertexSrc, FlatColorShaderFragmentSrc);

    auto textureShader =
        m_ShaderLibrary.load("sandbox/assets/shaders/Texture.glsl");
    m_Texture =
        wen::texture2D::create("sandbox/assets/textures/Checkerboard.png");
    m_ChernoLogoTexture =
        wen::texture2D::create("sandbox/assets/textures/ChernoLogo.png");
    textureShader->bind();
    std::dynamic_pointer_cast<wen::openglShader>(textureShader)
        ->uploadUniformInt("u_Texture", 0);
}

void exampleLayer::OnUpdate(wen::timeStep ts) {
    m_CameraController.onUpdate(ts); 

    wen::renderCommand::setClearColor({0.45f, 0.55f, 0.65f, 1.0f});
    wen::renderCommand::clear();

    wen::renderer::beginScene(m_CameraController.getCamera());
    static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
    std::dynamic_pointer_cast<wen::openglShader>(m_FlatColorShader)->bind();
    std::dynamic_pointer_cast<wen::openglShader>(m_FlatColorShader)
        ->uploadUniformFloat3("u_Color", m_SquareColor);
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
            wen::renderer::submit(m_FlatColorShader, m_SquareVA, transform);
        }
    }
    auto textureShader = m_ShaderLibrary.get("Texture");
    m_Texture->bind();
    wen::renderer::submit(textureShader, m_SquareVA,
                          glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
    m_ChernoLogoTexture->bind();
    wen::renderer::submit(textureShader, m_SquareVA,
                          glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
    wen::renderer::endScene();
}

void exampleLayer::OnEvent(wen::event& event) {
    m_CameraController.onEvent(event);
}

void exampleLayer::OnImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}