#pragma once

#include "wen/core/window.hpp"
#include "wen/events/applicationEvent.hpp"
#include "wen/core/layerStack.hpp"
#include "wen/imgui/imguiLayer.hpp"
#include "wen/core/base.hpp"

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
    Scope<window> m_Window;
    bool m_Running = true;
    bool m_Minimized = false;
    layerStack m_LayerStack;
    imguiLayer* m_imguiLayer;
    float m_LastFrameTime = 0.0f;

private:
    static application* s_Instance;
};

application* CreateApplication();
}  // namespace wen