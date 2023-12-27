#include "gameLayer.hpp"
#include "random.hpp"

gameLayer::gameLayer() : layer("gameLayer") {
    auto& window = wen::application::get().getWindow();
    createCamera(window.getWidth(), window.getHeight());

    random::init();
}

void gameLayer::OnAttach() {
    // 初始化场景
    m_Level.init();
    // 初始化字体
    ImGuiIO& io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF(
        "sandbox/assets/fonts/opensans/OpenSans-Regular.ttf", 120.0f);
}

void gameLayer::OnDetach() {
    layer::OnDetach();
}

void gameLayer::OnUpdate(wen::timeStep ts) {
    // 记录运行的时间
    m_Time += ts;

    // 10ms % 8 = 2 20ms % 8 = 4 30ms % 8 = 6 40ms % 8 = 0
    // 0-2ms不闪 2ms-4ms闪烁 4ms-6ms不闪 6ms-8ms闪烁
    // 计算闪烁的状态，4ms为一个周期
    if ((int)(m_Time * 10.0f) % 8 > 4) {
        m_Blink = !m_Blink;
    }

    // 游戏结束
    if (m_Level.isGameOver()) {
        WEN_INFO("GameLayer::OnUpdate isGameOver: true");
        m_State = gameState::GameOver;
    }

    // 更新相机位置，以player为中心
    const auto& playerPos = m_Level.getPlayer().getPosition();
    m_Camera->setPosition({playerPos.x, playerPos.y, 0.0f});

    // 更新场景
    switch (m_State) {
        case gameState::Play:
            m_Level.OnUpdate(ts);
            break;
        case gameState::MainMenu:
            break;
        case gameState::GameOver:
            break;
    }

    // 渲染场景
    wen::renderCommand::setClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    wen::renderCommand::clear();
    wen::renderer2D::beginScene(*m_Camera);
    m_Level.OnRender();
    wen::renderer2D::endScene();
}

void gameLayer::OnImGuiRender() {
    switch (m_State) {
        case gameState::Play: {
            WEN_INFO("gameLayer::OnImGuiRender Play");
            uint32_t playerScore = m_Level.getPlayer().getScore();
            std::string scoreStr =
                std::string("Score: ") + std::to_string(playerScore);
            // 将分数显示在左上角
            ImGui::GetForegroundDrawList()->AddText(
                m_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff,
                scoreStr.c_str());
            break;
        }
        case gameState::MainMenu: {
            WEN_INFO("gameLayer::OnImGuiRender MainMenu");
            auto pos = ImGui::GetWindowPos();
            auto width = wen::application::get().getWindow().getWidth();

            // 将文字显示在屏幕中央，从中间的位置向左偏移300，向下偏移50开始显示
            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;
            if (m_Blink) {
                ImGui::GetForegroundDrawList()->AddText(
                    m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
            }
            break;
        }
        case gameState::GameOver: {
            WEN_INFO("gameLayer::OnImGuiRender GameOver");
            auto pos = ImGui::GetWindowPos();
            auto width = wen::application::get().getWindow().getWidth();
            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;
            if (m_Blink) {
                ImGui::GetForegroundDrawList()->AddText(
                    m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
            }

            pos.x += 200.0f;
            pos.y += 150.0f;
            uint32_t playerScore = m_Level.getPlayer().getScore();
            std::string scoreStr =
                std::string("Score: ") + std::to_string(playerScore);
            ImGui::GetForegroundDrawList()->AddText(
                m_Font, 48.0f, pos, 0xffffffff, scoreStr.c_str());
            break;
        }
    }
}

void gameLayer::OnEvent(wen::event& e) {
    wen::eventDispatcher dispatcher(e);
    dispatcher.dispatch<wen::windowResizeEvent>(
        BIND_EVENT_FN(gameLayer::OnWindowResize));
    dispatcher.dispatch<wen::mouseButtonPressedEvent>(
        BIND_EVENT_FN(gameLayer::OnMouseButtonPressed));
}

bool gameLayer::OnMouseButtonPressed(wen::mouseButtonPressedEvent& e) {
    if (m_State == gameState::GameOver) {
        m_Level.reset();
    }
    m_State = gameState::Play;
    return false;
}

bool gameLayer::OnWindowResize(wen::windowResizeEvent& e) {
    createCamera(e.getWidth(), e.getHeight());
    return false;
}

// left: -8*16/9 right: 8*16/9 bottom: -8 top: 8
void gameLayer::createCamera(uint32_t width, uint32_t height) {
    float aspectRatio = (float)width / (float)height;
    float camWidth = 8.0f;
    float bottom = -camWidth;
    float top = camWidth;
    float left = bottom * aspectRatio;
    float right = top * aspectRatio;
    m_Camera =
        wen::CreateScope<wen::orthographicCamera>(left, right, bottom, top);
}