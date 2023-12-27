#include "level.hpp"
#include "random.hpp"
#include <glm/gtc/matrix_transform.hpp>

static glm::vec4 HSVtoRGB(const glm::vec3& hsv) {
    int H = (int)(hsv.x * 360.0f);
    double S = hsv.y;
    double V = hsv.z;

    double C = S * V;
    double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    double m = V - C;
    double Rs = 0, Gs = 0, Bs = 0;

    if (H >= 0 && H < 60) {
        Rs = C;
        Gs = X;
        Bs = 0;
    } else if (H >= 60 && H < 120) {
        Rs = X;
        Gs = C;
        Bs = 0;
    } else if (H >= 120 && H < 180) {
        Rs = 0;
        Gs = C;
        Bs = X;
    } else if (H >= 180 && H < 240) {
        Rs = 0;
        Gs = X;
        Bs = C;
    } else if (H >= 240 && H < 300) {
        Rs = X;
        Gs = 0;
        Bs = C;
    } else {
        Gs = C;
        Gs = 0;
        Bs = X;
    }
    return {(Rs + m), (Gs + m), (Bs + m), 1.0f};
}

// 计算三角形p p0 p1的面积是否 > p0 p1 p2的面积
static bool PointInTri(const glm::vec2& p, glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2) {
    // 叉乘计算三角形p0 p2 p的面积和方向
    float s = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
    // 叉乘计算三角形p0 p1 p的面积和方向
    float t = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y;
    // 判断这两个三角形面积的方向是否一致，不一致直接返回false
    if ((s < 0) != (t < 0)) return false;

    // 叉乘计算三角形p0 p1 p2的面积
    float A = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;

    // 两个三角形面积s t之和 < A时，则点p在三角形内
    // 注意考虑A的方向
    return A < 0 ? (s <= 0 && s + t >= A) : (s >= 0 && s + t <= A);
}

void level::init() {
    m_TriangleTexture =
        wen::texture2D::create("sandbox/assets/textures/Triangle.png");
    m_Player.loadAssets();
    m_Pillars.resize(5);
    for (int i = 0; i < 5; i++) {
        createPillar(i, i * 10.0f);
    }
}

void level::OnUpdate(wen::timeStep ts) {
    m_Player.OnUpdate(ts);
    if (collisionTest()) {
        WEN_INFO("level::OnUpdate: true");
        gameOver();
        return;
    }

    // HSV.x 通道表示颜色种类
    m_PillarHSV.x += 0.1f * ts;
    if (m_PillarHSV.x > 1.0f) {
        m_PillarHSV.x = 0.0f;
    }

    // player.x > pillar.x时，创建新的pillar
    if (m_Player.getPosition().x > m_PillarTarget) {
        createPillar(m_PillarIndex, m_PillarTarget + 20.0f);
        // 更新m_PillarIndex，通过取模，一直能循环使用缓存的Pillar
        m_PillarIndex = ++m_PillarIndex % m_Pillars.size();
        // 更新小汽车前面的柱子的坐标，表示为m_PillarTarget
        m_PillarTarget += 10.0f;
    }
}

void level::OnRender() {
    const auto& playerPos = m_Player.getPosition();
    glm::vec4 color = HSVtoRGB(m_PillarHSV);

    wen::renderer2D::drawQuad({playerPos.x, 0.0f, -0.8f}, {50.0f, 50.0f},
                              {0.3f, 0.3f, 0.3f, 1.0f});

    wen::renderer2D::drawQuad({playerPos.x, 34.0f}, {50.0f, 50.0f}, color);
    wen::renderer2D::drawQuad({playerPos.x, -34.0f}, {50.0f, 50.0f}, color);

    for (auto& pillar : m_Pillars) {
        wen::renderer2D::drawQuad(pillar.topPosition, pillar.topScale,
                                  glm::radians(180.0f), m_TriangleTexture, 1.0f,
                                  color);
        wen::renderer2D::drawQuad(pillar.bottomPosition, pillar.bottomScale,
                                  0.0f, m_TriangleTexture, 1.0f, color);
    }
    m_Player.OnRender();
}

void level::OnImGuiRender() {
    m_Player.OnImGuiRender();
}

void level::createPillar(int index, float offset) {
    pillar& pillar = m_Pillars[index];
    pillar.topPosition.x = offset;
    pillar.bottomPosition.x = offset;
    pillar.topPosition.z = index * 0.1f - 0.5f;
    pillar.bottomPosition.z = index * 0.1f - 0.5f + 0.5f;

    float center = random::Float() * 35.0f - 17.5f;
    float gap = 5.0f + random::Float() * 5.0f;

    pillar.topPosition.y = 10.0f - ((10.0f - center) * 0.2f) + gap * 0.5f;
    pillar.bottomPosition.y = -10.0f - ((-10.0f - center) * 0.2f) - gap * 0.5f;
}

bool level::collisionTest() {
    if (glm::abs(m_Player.getPosition().y) > 8.5f) {
        return true;
    }
    glm::vec4 playerVertices[4] = {
        {-0.5f, -0.5f, 0.0f, 1.0f},
        { 0.5f, -0.5f, 0.0f, 1.0f},
        { 0.5f,  0.5f, 0.0f, 1.0f},
        {-0.5f,  0.5f, 0.0f, 1.0f},
    };

    const auto& pos = m_Player.getPosition();
    glm::vec4 playerTransformedVerts[4];
    for (int i = 0; i < 4; i++) {
        playerTransformedVerts[i] =
            glm::translate(glm::mat4(1.0f), {pos.x, pos.y, 0.0f}) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Player.getRotation()),
                        {0.0f, 0.0f, 1.0f}) *
            glm::scale(glm::mat4(1.0f), {1.0f, 1.3f, 1.0f}) * playerVertices[i];
    }

    // To match  triangle.png (each corner is 10% from the texture edge)
    glm::vec4 pillarVertices[3] = {
        {-0.5f + 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
        { 0.5f - 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
        { 0.0f + 0.0f,  0.5f - 0.1f, 0.0f, 1.0f},
    };

    for (auto& p : m_Pillars) {
        glm::vec2 tri[3];

        // Top pillars
        for (int i = 0; i < 3; i++) {
            tri[i] = glm::translate(glm::mat4(1.0f),
                                    {p.topPosition.x, p.topPosition.y, 0.0f}) *
                     glm::rotate(glm::mat4(1.0f), glm::radians(180.0f),
                                 {0.0f, 0.0f, 1.0f}) *
                     glm::scale(glm::mat4(1.0f),
                                {p.topScale.x, p.topScale.y, 1.0f}) *
                     pillarVertices[i];
        }

        for (auto& vert : playerTransformedVerts) {
            if (PointInTri({vert.x, vert.y}, tri[0], tri[1], tri[2])) {
                return true;
            }
        }

        // Bottom pillars
        for (int i = 0; i < 3; i++) {
            tri[i] =
                glm::translate(glm::mat4(1.0f),
                               {p.bottomPosition.x, p.bottomPosition.y, 0.0f}) *
                glm::scale(glm::mat4(1.0f),
                           {p.bottomScale.x, p.bottomScale.y, 1.0f}) *
                pillarVertices[i];
        }

        for (auto& vert : playerTransformedVerts) {
            if (PointInTri({vert.x, vert.y}, tri[0], tri[1], tri[2])) {
                return true;
            }
        }
    }
    return false;
}

void level::gameOver() {
    WEN_INFO("level::gameOver()");
    m_GameOver = true;
}

void level::reset() {
    m_GameOver = false;
    m_Player.reset();
    m_PillarTarget = 30.0f;
    m_PillarIndex = 0;
    for (int i = 0; i < 5; i++) {
        createPillar(i, i * 10.0f);
    }
}