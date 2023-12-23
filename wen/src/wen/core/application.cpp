#include "wen/core/application.hpp"
#include "wen/core/log.hpp"
#include "wen/events/applicationEvent.hpp"
#include "wen/core/timeStep.hpp"
#include "wen/renderer/renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace wen {
application *application::s_Instance = nullptr;

application::application() {
    s_Instance = this;
    m_Window = Scope<window>(window::create());
    m_Window->setEventCallback(BIND_EVENT_FN(application::onEvent));
    renderer::init();
    m_imguiLayer = new imguiLayer();
    pushOverlay(m_imguiLayer);
}

application::~application() {}

void application::onEvent(event &e) {
    eventDispatcher dispatcher(e);
    dispatcher.dispatch<windowCloseEvent>(
        BIND_EVENT_FN(application::onWindowClose));
    dispatcher.dispatch<windowResizeEvent>(
        BIND_EVENT_FN(application::onWindowResize));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(e);
        if (e.handled) {
            break;
        }
    }
}

void application::Run() {
    while (m_Running) {
        float time = glfwGetTime();
        timeStep timestep = time - m_LastFrameTime;
        m_LastFrameTime = time;
        // std::cout << "FPS: " << 1.0f / timestep.getSeconds() << std::endl;

        for (layer *layer : m_LayerStack) {
            layer->OnUpdate(timestep);
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
        m_Minimized = true;
        return false;
    }
    m_Minimized = false;
    uint32_t pixelWidth = e.getWidth(), pixelHeight = e.getHeight();
    m_Window->getPixelSize(pixelWidth, pixelHeight);
    renderer::onWindowResize(pixelWidth, pixelHeight);
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