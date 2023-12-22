#pragma once

#include "wen/core/window.hpp"
#include "wen/events/applicationEvent.hpp"
#include "wen/core/layerStack.hpp"
#include "wen/imgui/imguiLayer.hpp"

namespace wen {
class application {
public:
    application();
    virtual ~application();

    void onEvent(event& e);
    void Run();

    void pushLayer(layer* layer);
    void pushOverlay(layer* overlay);

    inline window& getWindow() { return *m_Window; }

    inline static application& get() { return *s_Instance; }

private:
    bool onWindowClose(windowCloseEvent& e);
    bool onWindowResize(windowResizeEvent& e);

private:
    std::unique_ptr<window> m_Window;
    bool m_Running = true;
    layerStack m_LayerStack;
    static application* s_Instance;
    imguiLayer* m_imguiLayer;
};

application* CreateApplication();
}  // namespace wen