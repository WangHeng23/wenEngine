#pragma once 

#include "wen.hpp"

class sandbox2d : public wen::layer {
public:
    sandbox2d();
    virtual ~sandbox2d() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(wen::timeStep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(wen::event& e) override;

private:
    wen::orthographicCameraController m_cameraController;

    // Temp
    wen::Ref<wen::vertexArray> m_SquareVA;
    wen::Ref<wen::shader> m_FlatColorShader;
    wen::Ref<wen::texture2D> m_Texture;

    glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};