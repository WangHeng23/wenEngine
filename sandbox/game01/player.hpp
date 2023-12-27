#pragma once

#include "wen.hpp"
#include "particleSystem.hpp"
#include <glm/gtc/matrix_transform.hpp>

class player {
public:
    player();

    void loadAssets();
    void OnUpdate(wen::timeStep ts);
    void OnRender();
    void OnImGuiRender();
    void reset();

    float getRotation() { return m_Velocity.y * 4.0f - 90.0; }

    const glm::vec2& getPosition() const { return m_Position; }

    uint32_t getScore() const {
        return (uint32_t)(m_Position.x + 10.0f) / 10.0f;
    }

private:
    glm::vec2 m_Position = {-10.0f, 0.0f};
    glm::vec2 m_Velocity = {5.0f, 0.0f};

    float m_EnginePower = 0.5f;
    float m_Gravity = 0.4f;

    float m_Time = 0.0f;
    float m_SmokeEmitInterval = 0.4f;
    float m_SmokeNextEmitTime = m_SmokeEmitInterval;

    particleProps m_SmokeParticle, m_EngineParticle;
    particleSystem m_ParticleSystem;

    wen::Ref<wen::texture2D> m_ShipTexture;
};