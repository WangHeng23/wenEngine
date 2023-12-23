#pragma once
#include "wen.hpp"

class exampleLayer : public wen::layer {
public:
    exampleLayer();

    void OnUpdate(wen::timeStep ts) override;
    void OnEvent(wen::event& event) override;
    void OnImGuiRender() override;

private:
    wen::shaderLibrary m_ShaderLibrary;
    wen::Ref<wen::shader> m_Shader;
    wen::Ref<wen::vertexArray> m_VertexArray;
    wen::Ref<wen::shader> m_FlatColorShader;
    wen::Ref<wen::vertexArray> m_SquareVA;
    wen::Ref<wen::texture2D> m_Texture, m_ChernoLogoTexture;

    wen::orthographicCameraController m_CameraController;
    
    glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};
};