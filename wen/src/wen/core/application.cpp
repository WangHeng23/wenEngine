#include "wen/core/application.hpp"
#include "wen/core/log.hpp"
#include "wen/events/applicationEvent.hpp"
#include "wen/renderer/renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace wen {
application *application::s_Instance = nullptr;

application::application() {
    s_Instance = this;
    m_Window = std::unique_ptr<window>(window::create());
    m_Window->setEventCallback(BIND_EVENT_FN(application::onEvent));
    m_imguiLayer = new imguiLayer();
    pushOverlay(m_imguiLayer);

    // -----------triangle------------
    // 创建顶点数组
    m_VertexArray.reset(vertexArray::create());
    float vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                             0.5f,  -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                             0.0f,  0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f};
    // 创建顶点缓冲区
    std::shared_ptr<vertexBuffer> vertexBuffer_(
        vertexBuffer::create(vertices, sizeof(vertices)));
    // 创建顶点缓冲区布局
    bufferLayout layout = {
        {shaderDataType::float3, "a_Position"},
        {shaderDataType::float4,    "a_Color"}
    };
    // 设置顶点缓冲区布局
    vertexBuffer_->setLayout(layout);
    // 将顶点缓冲区添加到顶点数组
    m_VertexArray->addVertexBuffer(vertexBuffer_);
    uint32_t indices[3] = {0, 1, 2};
    // 创建索引缓冲区
    std::shared_ptr<indexBuffer> indexBuffer_(
        indexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
    // 将索引缓冲区添加到顶点数组
    m_VertexArray->setIndexBuffer(indexBuffer_);
    // ------------------------------

    // ----------square--------------
    m_SquareVA.reset(vertexArray::create());
    float squareVertices[3 * 4] = {
        -0.5f, -0.5f, 0.0f, 
        0.5f,  -0.5f, 0.0f,
        0.5f,  0.5f,  0.0f,
        -0.5f, 0.5f, 0.0f
    };
    std::shared_ptr<vertexBuffer> squareVB(
        vertexBuffer::create(squareVertices, sizeof(squareVertices)));
    squareVB->setLayout({
        {shaderDataType::float3, "a_Position"}
    });
    m_SquareVA->addVertexBuffer(squareVB);
    uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
    std::shared_ptr<indexBuffer> squareIB(indexBuffer::create(
        squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_SquareVA->setIndexBuffer(squareIB);
    // ------------------------------

    std::string vertexSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec4 a_Color;
    out vec3 v_Position;
    out vec4 v_Color;
    void main()
    {
        v_Position = a_Position;
        v_Color = a_Color;
        gl_Position = vec4(a_Position, 1.0);
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

    m_Shader.reset(new shader(vertexSrc, fragmentSrc));

    std::string blueShaderVertexSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 a_Position;
    out vec3 v_Position;
    void main()
    {
        v_Position = a_Position;
        gl_Position = vec4(a_Position, 1.0);
    }
)";
    std::string blueShaderFragmentSrc = R"(
    #version 330 core
    layout(location = 0) out vec4 color;
    in vec3 v_Position;
    uniform vec3 u_Color;
    void main()
    {
        color = vec4(0.2, 0.3, 0.8, 1.0);
    }
)";
    m_BlueShader.reset(new shader(blueShaderVertexSrc, blueShaderFragmentSrc));
}

application::~application() {}

void application::onEvent(event &e) {
    eventDispatcher dispatcher(e);
    dispatcher.dispatch<windowCloseEvent>(
        BIND_EVENT_FN(application::onWindowClose));
    dispatcher.dispatch<windowResizeEvent>(
        BIND_EVENT_FN(application::onWindowResize));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(e);
        if (e.handled) {
            break;
        }
    }
}

void application::Run() {
    while (m_Running) {
        rendererCommand::setClearColor({0.45f, 0.55f, 0.65f, 1.0f});
        rendererCommand::clear();

        renderer::beginScene();
        m_BlueShader->bind();
        renderer::submit(m_SquareVA);
        m_Shader->bind();
        renderer::submit(m_VertexArray);
        renderer::endScene();

        for (layer *layer : m_LayerStack) {
            layer->OnUpdate();
        }
        m_imguiLayer->begin();
        for (layer *layer : m_LayerStack) {
            layer->OnImGuiRender();
        }
        m_imguiLayer->end();
        m_Window->onUpdate();
    }
}

bool application::onWindowClose(windowCloseEvent &e) {
    m_Running = false;
    return true;
}

bool application::onWindowResize(windowResizeEvent &e) {
    if (e.getWidth() == 0 || e.getHeight() == 0) {
        return false;
    }
    WEN_TRACE("WindowResizeEvent: ({0}, {1})", e.getWidth(), e.getHeight());
    return false;
}

void application::pushLayer(layer *layer) {
    m_LayerStack.pushLayer(layer);
    layer->OnAttach();
}

void application::pushOverlay(layer *overlay) {
    m_LayerStack.pushOverlay(overlay);
    overlay->OnAttach();
}
}  // namespace wen