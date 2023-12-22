#pragma once
#include "wen.hpp"

class exampleLayer : public wen::layer {
public:
    exampleLayer();

    void OnUpdate() override;
    void OnEvent(wen::event& event) override;
    void OnImGuiRender() override;

private:
};