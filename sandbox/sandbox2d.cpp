#include "sandbox2d.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

sandbox2d::sandbox2d()
    : layer("sandbox2d"), m_cameraController(1600.f / 900.f, true) {
    m_Texture =
        wen::texture2D::create("sandbox/assets/textures/Checkerboard.png");
}

void sandbox2d::OnAttach() {
    WEN_PROFILE_FUNCTION();
}

void sandbox2d::OnDetach() {
    WEN_PROFILE_FUNCTION();
}

void sandbox2d::OnUpdate(wen::timeStep ts) {
    // Update
    m_cameraController.onUpdate(ts);

    wen::renderer2D::resetStats();

    // Render
    {
        WEN_PROFILE_SCOPE("renderer prep");
        wen::renderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
        wen::renderCommand::clear();
    }

    {
        WEN_PROFILE_SCOPE("renderer draw");
        wen::renderer2D::beginScene(m_cameraController.getCamera());
        wen::renderer2D::drawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, m_SquareColor);
        wen::renderer2D::drawQuad({0.5f, -0.5f}, {0.5f, 0.8f},
                                  {0.2f, 0.8f, 0.9f, 1.0f});
        wen::renderer2D::drawQuad({-10.0f, -10.0f, -0.1f}, {20.0f, 20.0f},
                                  m_Texture, 10.0f);
        wen::renderer2D::drawRotatedQuad({-0.5f, -1.5f, 0.0f}, {1.0f, 1.0f}, 45,
                                         m_Texture, 20.0f);
        wen::renderer2D::endScene();

        wen::renderer2D::beginScene(m_cameraController.getCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.1f) {
            for (float x = -5.0f; x < 5.0f; x += 0.1f) {
                glm::vec4 color = {(x + 5.0f) / 10.0f, 0.4f,
                                   (y + 5.0f) / 10.0f, 0.5f};
                wen::renderer2D::drawQuad({x, y}, {0.45f, 0.45f}, color);
            }
        }
        wen::renderer2D::endScene();
    }
}

void sandbox2d::OnImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

    auto stats = wen::renderer2D::getStats();
    ImGui::Text("Draw Calls: %d", stats.drawCalls);
    ImGui::Text("Quad Count: %d", stats.quadCount);
    ImGui::Text("Vertex Count: %d", stats.getTotalVertexCount());
    ImGui::Text("Index Count: %d", stats.getTotalIndexCount());

    ImGui::End();
}

void sandbox2d::OnEvent(wen::event& e) {
    m_cameraController.onEvent(e);
}