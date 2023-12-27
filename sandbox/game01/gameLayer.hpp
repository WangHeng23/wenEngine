#pragma once

#include "level.hpp"
#include "wen.hpp"

class gameLayer : public wen::layer {
public:
    gameLayer();
    ~gameLayer() override = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(wen::timeStep ts) override;
    void OnImGuiRender() override;
    void OnEvent(wen::event& e) override;

    bool OnMouseButtonPressed(wen::mouseButtonPressedEvent& e);
    bool OnWindowResize(wen::windowResizeEvent& e);

private:
    void createCamera(uint32_t width, uint32_t height);

private:
    // Camera
    wen::Scope<wen::orthographicCamera> m_Camera;
    // level
    level m_Level;
    // font
    ImFont* m_Font{};
    // update time
    float m_Time = 0.0f;
    // blink
    bool m_Blink = false;
    // game state
    enum class gameState { Play, MainMenu, GameOver };
    gameState m_State = gameState::MainMenu;
};