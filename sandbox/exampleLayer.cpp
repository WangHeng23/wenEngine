#include "exampleLayer.hpp"
#include "pch.hpp"

exampleLayer::exampleLayer() : layer("example") {}

void exampleLayer::OnUpdate() {
    if (wen::input::IsKeyPressed(WEN_KEY_TAB)) {
        WEN_TRACE("Tab key is pressed (poll)!");
    }
    if (wen::input::IsMouseButtonPressed(wen::mouse::button1)) {
        WEN_TRACE("Mouse button 1 is pressed (poll)!");
    }
}

void exampleLayer::OnEvent(wen::event& event) {
    if (event.getEventType() == wen::eventType::keyPressed) {
        wen::keyPressedEvent& e = (wen::keyPressedEvent&)event;
        if (e.getKeyCode() == WEN_KEY_A) {
            WEN_TRACE("A key is pressed (event)!");
        }
        WEN_TRACE("{0}", (char)e.getKeyCode());
    }
}

void exampleLayer::OnImGuiRender() {
    ImGui::Begin("Test");
    ImGui::Text("Hello World");
    ImGui::End();
}