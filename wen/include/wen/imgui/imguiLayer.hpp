#pragma once

#include "wen/core/layer.hpp"
#include "wen/events/applicationEvent.hpp"
#include "wen/events/keyEvent.hpp"
#include "wen/events/mouseEvent.hpp"

namespace wen {
class imguiLayer : public layer {
public:
    imguiLayer();
    ~imguiLayer() = default;

    void OnAttach() override;
    void OnDetach() override;

    // void OnImGuiRender() override;

    void begin();
    void end();

    void BlockEvents(bool block) { m_BlockEvents = block; }

private:
    bool m_BlockEvents = true;
};
}  // namespace wen