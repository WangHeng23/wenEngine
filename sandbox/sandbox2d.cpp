#include "sandbox2d.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

sandbox2d::sandbox2d()
    : layer("sandbox2d"), m_cameraController(1600.f / 900.f, true) {}

void sandbox2d::OnAttach() {
    WEN_PROFILE_FUNCTION();
    m_Texture =
        wen::texture2D::create("sandbox/assets/textures/Checkerboard.png");
    m_SquareVA = wen::vertexArray::create();

    float squareVertices[5 * 4] = {
        -0.5, -0.5f, 0.0f, 0.5, -0.5f, 0.0f, 0.5, 0.5f, 0.0f, -0.5, 0.5f, 0.0f,
    };

    wen::Ref<wen::vertexBuffer> squareVB(
        wen::vertexBuffer::create(squareVertices, sizeof(squareVertices)));
    squareVB->setLayout({
        {wen::shaderDataType::float3, "a_Position"},
    });
    m_SquareVA->addVertexBuffer(squareVB);

    uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
    wen::Ref<wen::indexBuffer> squareIB(wen::indexBuffer::create(
        squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_SquareVA->setIndexBuffer(squareIB);

    m_FlatColorShader =
        wen::shader::create("sandbox/assets/shaders/FlatColor.glsl");
}

void sandbox2d::OnDetach() {
    WEN_PROFILE_FUNCTION();
}

void sandbox2d::OnUpdate(wen::timeStep ts) {
    // Update
    m_cameraController.onUpdate(ts);

    // Render
    {
        WEN_PROFILE_SCOPE("renderer prep");
        wen::renderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
        wen::renderCommand::clear();
    }

    {
        WEN_PROFILE_SCOPE("renderer draw");
        wen::renderer2D::beginScene(m_cameraController.getCamera());
        wen::renderer2D::drawQuad({-1.0f, 0.0f}, {0.8f, 0.8f},
                                  glm::radians(-45.0f), m_SquareColor);
        wen::renderer2D::drawQuad({0.5f, 0.0f}, {1.0f, 1.0f},
                                  {0.8f, 0.2f, 0.3f, 1.0f});
        wen::renderer2D::drawQuad({0.0f, 0.0f, -0.1f}, {5.0f, 5.0f}, m_Texture,
                                  10.0f);
        wen::renderer2D::endScene();
    }
}

void sandbox2d::OnImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void sandbox2d::OnEvent(wen::event& e) {
    m_cameraController.onEvent(e);
}