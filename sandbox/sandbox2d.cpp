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
        wen::renderer2D::drawQuad({0.5f, -0.5f}, {0.5f, 0.75f},
                                  {0.8f, 0.3f, 0.2f, 1.0f});
        wen::renderer2D::drawQuad({-5.0f, -5.0f, -0.1f}, {10.0f, 10.0f},
                                  m_Texture, 5.0f);
        wen::renderer2D::drawQuad({-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f},
                                  m_Texture, 20.0f);
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