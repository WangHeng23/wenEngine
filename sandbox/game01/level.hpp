#pragma once 

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "wen.hpp"
#include <vector>
#include "player.hpp"

struct pillar {
    glm::vec3 topPosition{0.0f, 10.0f, 0.0f};
    glm::vec2 topScale{15.0f, 20.0f};
    glm::vec3 bottomPosition{10.0f, 0.0f, 0.0f};
    glm::vec2 bottomScale{15.0f, 20.0f};
};

class level {
public:
    void init();
    void OnUpdate(wen::timeStep ts);
    void OnRender();
    void OnImGuiRender();
    bool isGameOver() const { return m_GameOver; }
    void reset();

    player& getPlayer() { return m_Player; }

private:
    void createPillar(int index, float offset);
    bool collisionTest();
    void gameOver();

private:
    player m_Player;
    bool m_GameOver = false;
    float m_PillarTarget = 30.0f;
    int m_PillarIndex = 0;
    glm::vec3 m_PillarHSV{0.0f, 0.8f, 0.8f};
    std::vector<pillar> m_Pillars;
    wen::Ref<wen::texture2D> m_TriangleTexture;
};