#include "particleSystem.hpp"

#include "random.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

particleSystem::particleSystem() {
    m_ParticlePool.resize(1000);
}

void particleSystem::Emit(const particleProps& particleProps) {
    particle& particle = m_ParticlePool[m_PoolIndex];
    particle.Active = true;
    particle.Position = particleProps.Position;
    particle.Rotation = random::Float() * 2.0f * glm::pi<float>();

    // Velocity
    particle.Velocity = particleProps.Velocity;
    particle.Velocity.x +=
        particleProps.VelocityVariation.x * (random::Float() - 0.5f);
    particle.Velocity.y +=
        particleProps.VelocityVariation.y * (random::Float() - 0.5f);

    // Color
    particle.ColorBegin = particleProps.ColorBegin;
    particle.ColorEnd = particleProps.ColorEnd;

    // Size
    particle.SizeBegin = particleProps.SizeBegin +
                         particleProps.SizeVariation * (random::Float() - 0.5f);
    particle.SizeEnd = particleProps.SizeEnd;

    // Life
    particle.LifeTime = particleProps.LifeTime;
    particle.LifeRemaining = particleProps.LifeTime;

    m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}

void particleSystem::OnUpdate(wen::timeStep ts) {
    for (auto& particle : m_ParticlePool) {
        if (!particle.Active) {
            continue;
        }

        if (particle.LifeRemaining <= 0.0f) {
            particle.Active = false;
            continue;
        }

        particle.LifeRemaining -= ts;
        particle.Position += particle.Velocity * (float)ts;
        particle.Rotation += 0.01f * ts;
    }
}

void particleSystem::OnRender() {
    for (auto& particle : m_ParticlePool) {
        if (!particle.Active) {
            continue;
        }
        float life = particle.LifeRemaining / particle.LifeTime;
        glm::vec4 color =
            glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
        color.a = color.a * life;

        float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
        wen::renderer2D::drawRotatedQuad(particle.Position, {size, size},
                                  particle.Rotation, color);
    }
}