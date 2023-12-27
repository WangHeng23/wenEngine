#pragma once

#include "wen.hpp"

struct particleProps {
    glm::vec2 Position;
    glm::vec2 Velocity, VelocityVariation;
    glm::vec4 ColorBegin, ColorEnd;
    float SizeBegin, SizeEnd, SizeVariation;
    float LifeTime = 1.0f;
};

class particleSystem {
public:
    particleSystem();

    void Emit(const particleProps& particleProps);
    void OnUpdate(wen::timeStep ts);
    void OnRender();

private:
    struct particle {
        glm::vec2 Position;
        glm::vec2 Velocity;
        glm::vec4 ColorBegin, ColorEnd;
        float Rotation = 0.0f;
        float SizeBegin, SizeEnd;

        float LifeTime = 1.0f;
        float LifeRemaining = 0.0f;

        bool Active = false;
    };

    std::vector<particle> m_ParticlePool;
    uint32_t m_PoolIndex = 999;
};