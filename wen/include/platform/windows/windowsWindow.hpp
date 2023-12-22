#pragma once

#include "wen/core/window.hpp"
#include "wen/renderer/graphicsContext.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace wen {
class windowsWindow : public window {
public:
    windowsWindow(const windowProps &props);
    virtual ~windowsWindow();

    void onUpdate() override;

    uint32_t getWidth() const override { return m_Data.width; }

    uint32_t getHeight() const override { return m_Data.height; }

    void setEventCallback(const eventCallbackFn &callback) override {
        m_Data.eventCallback = callback;
    }

    void setVSync(bool enabled) override;
    bool isVSync() const override;

    void *getNativeWindow() const override { return m_Window; }

private:
    virtual void init(const windowProps &props);
    virtual void shutdown();

private:
    GLFWwindow *m_Window;
    graphicsContext* m_Context;

    struct windowData {
        std::string title;
        uint32_t width;
        uint32_t height;
        bool vSync;

        eventCallbackFn eventCallback;
    };

    windowData m_Data;
};
}  // namespace wen