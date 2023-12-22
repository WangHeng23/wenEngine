#include "wen/core/log.hpp"
#include "wen/core/application.hpp"
#include "wen/events/applicationEvent.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace wen {
application *application::s_Instance = nullptr;

application::application() {
    s_Instance = this;
    m_Window = std::unique_ptr<window>(window::create());
    m_Window->setEventCallback(BIND_EVENT_FN(application::onEvent));
    m_imguiLayer = new imguiLayer();
    pushOverlay(m_imguiLayer);
}

application::~application() {}

void application::onEvent(event &e) {
    eventDispatcher dispatcher(e);
    dispatcher.dispatch<windowCloseEvent>(BIND_EVENT_FN(application::onWindowClose));
    dispatcher.dispatch<windowResizeEvent>(BIND_EVENT_FN(application::onWindowResize));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(e);
        if (e.handled) {
            break;
        }
    }
}

void application::Run() {
    while (m_Running) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);  // NOLINT
        for (layer *layer : m_LayerStack) {
            layer->OnUpdate();
        }
        m_imguiLayer->begin();
        for (layer *layer : m_LayerStack) {
            layer->OnImGuiRender();
        }
        m_imguiLayer->end();
        m_Window->onUpdate();
    }
}

bool application::onWindowClose(windowCloseEvent &e) {
    m_Running = false;
    return true;
}

bool application::onWindowResize(windowResizeEvent &e) {
    if (e.getWidth() == 0 || e.getHeight() == 0) {
        return false;
    }
    WEN_TRACE("WindowResizeEvent: ({0}, {1})", e.getWidth(), e.getHeight());
    return false;
}

void application::pushLayer(layer *layer) {
    m_LayerStack.pushLayer(layer);
    layer->OnAttach();
}

void application::pushOverlay(layer *overlay) {
    m_LayerStack.pushOverlay(overlay);
    overlay->OnAttach();
}
}  // namespace wen